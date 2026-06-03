#include "mytcpserver.h"

#include "algorithms.h"
#include "database.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QHostAddress>
#include <QDebug>

MyTcpServer::MyTcpServer(QObject *parent)
    : QTcpServer(parent)
{
    connect(this, &QTcpServer::newConnection,
            this, &MyTcpServer::onNewConnection);
}

bool MyTcpServer::start(quint16 port)
{
    bool ok = listen(QHostAddress::Any, port);

    if (!ok) {
        qDebug() << "Server listen error:" << errorString();
        return false;
    }

    qDebug() << "TCP server started on port" << port;
    return true;
}

void MyTcpServer::onNewConnection()
{
    while (hasPendingConnections()) {
        QTcpSocket *socket = nextPendingConnection();

        connect(socket, &QTcpSocket::readyRead,
                this, &MyTcpServer::onReadyRead);

        connect(socket, &QTcpSocket::disconnected,
                this, &MyTcpServer::onClientDisconnected);

        qDebug() << "Client connected:" << socket->peerAddress().toString()
                 << socket->peerPort();
    }
}

void MyTcpServer::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket) {
        return;
    }

    QByteArray data = socket->readAll().trimmed();

    if (data.isEmpty()) {
        return;
    }

    qDebug() << "Request received:" << data;

    QJsonParseError parseError;
    QJsonDocument requestDocument = QJsonDocument::fromJson(data, &parseError);

    QJsonObject response;

    if (parseError.error != QJsonParseError::NoError || !requestDocument.isObject()) {
        response = makeError("Invalid JSON request");
    } else {
        response = processRequest(requestDocument.object());
    }

    QJsonDocument responseDocument(response);
    QByteArray responseBytes = responseDocument.toJson(QJsonDocument::Compact);
    responseBytes.append('\n');

    socket->write(responseBytes);
    socket->flush();

    qDebug() << "Response sent:" << responseBytes;
}

void MyTcpServer::onClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket) {
        return;
    }

    qDebug() << "Client disconnected";
    socket->deleteLater();
}

QJsonObject MyTcpServer::makeError(const QString &message) const
{
    QJsonObject object;
    object["status"] = "error";
    object["message"] = message;
    return object;
}

QJsonObject MyTcpServer::makeOk() const
{
    QJsonObject object;
    object["status"] = "ok";
    return object;
}

QJsonObject MyTcpServer::processRequest(const QJsonObject &request)
{
    QString command = request.value("command").toString().toLower();
    QString login = request.value("login").toString();

    if (command.isEmpty()) {
        return makeError("Command is missing");
    }

    if (command == "ping") {
        QJsonObject response = makeOk();
        response["message"] = "Tema 8 C++ Qt server is working";
        return response;
    }

    if (command == "register") {
        QString userLogin = request.value("login").toString();
        QString password = request.value("password").toString();

        if (userLogin.isEmpty() || password.isEmpty()) {
            return makeError("Login and password are required");
        }

        bool ok = Database::instance().registerUser(userLogin, password);

        if (!ok) {
            return makeError("User already exists or database error");
        }

        QJsonObject response = makeOk();
        response["message"] = "User registered";
        response["login"] = userLogin;
        return response;
    }

    if (command == "login") {
        QString userLogin = request.value("login").toString();
        QString password = request.value("password").toString();

        if (userLogin.isEmpty() || password.isEmpty()) {
            return makeError("Login and password are required");
        }

        bool ok = Database::instance().checkLogin(userLogin, password);

        if (!ok) {
            return makeError("Invalid login or password");
        }

        QJsonObject response = makeOk();
        response["message"] = "Login successful";
        response["login"] = userLogin;
        return response;
    }

    if (command == "sha512") {
        QString text = request.value("text").toString();

        if (text.isEmpty()) {
            return makeError("Text is required");
        }

        QString result = Algorithms::sha512(text);

        Database::instance().saveOperation("sha512", text, result, login);

        QJsonObject response = makeOk();
        response["operation"] = "sha512";
        response["input"] = text;
        response["result"] = result;
        return response;
    }

    if (command == "rsa") {
        QString text = request.value("text").toString();

        if (text.isEmpty()) {
            return makeError("Text is required");
        }

        QString result = Algorithms::rsaDemoEncrypt(text);

        Database::instance().saveOperation("rsa", text, result, login);

        QJsonObject response = makeOk();
        response["operation"] = "rsa";
        response["input"] = text;
        response["result"] = result;
        return response;
    }

    if (command == "bisection") {
        double a = request.value("a").toDouble();
        double b = request.value("b").toDouble();
        int iterations = request.value("iterations").toInt(50);

        QJsonObject resultObject = Algorithms::bisection(a, b, iterations);

        QString inputText = QString("a=%1, b=%2, iterations=%3")
                                .arg(a)
                                .arg(b)
                                .arg(iterations);

        QString resultText = QString(QJsonDocument(resultObject).toJson(QJsonDocument::Compact));

        Database::instance().saveOperation("bisection", inputText, resultText, login);

        return resultObject;
    }

    if (command == "shortest_path") {
        int from = request.value("from").toInt();
        int to = request.value("to").toInt();

        QJsonObject resultObject = Algorithms::shortestPath(from, to);

        QString inputText = QString("from=%1, to=%2").arg(from).arg(to);
        QString resultText = QString(QJsonDocument(resultObject).toJson(QJsonDocument::Compact));

        Database::instance().saveOperation("shortest_path", inputText, resultText, login);

        return resultObject;
    }

    if (command == "history") {
        int limit = request.value("limit").toInt(20);

        QJsonObject response = makeOk();
        response["operation"] = "history";
        response["items"] = Database::instance().history(limit);
        return response;
    }

    return makeError("Unknown command: " + command);
}
