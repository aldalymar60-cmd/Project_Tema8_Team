#include <QCoreApplication>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

QJsonObject sendRequest(const QJsonObject &request)
{
    QTcpSocket socket;

    socket.connectToHost("127.0.0.1", 5555);

    if (!socket.waitForConnected(3000)) {
        QJsonObject error;
        error["status"] = "error";
        error["message"] = "Cannot connect to server";
        return error;
    }

    QJsonDocument requestDocument(request);
    QByteArray requestData = requestDocument.toJson(QJsonDocument::Compact);
    requestData.append('\n');

    socket.write(requestData);

    if (!socket.waitForBytesWritten(3000)) {
        QJsonObject error;
        error["status"] = "error";
        error["message"] = "Cannot send request";
        return error;
    }

    if (!socket.waitForReadyRead(3000)) {
        QJsonObject error;
        error["status"] = "error";
        error["message"] = "No response from server";
        return error;
    }

    QByteArray responseData = socket.readAll().trimmed();

    QJsonDocument responseDocument = QJsonDocument::fromJson(responseData);

    if (!responseDocument.isObject()) {
        QJsonObject error;
        error["status"] = "error";
        error["message"] = "Invalid JSON response";
        return error;
    }

    socket.disconnectFromHost();

    return responseDocument.object();
}

void printResponse(const QString &title, const QJsonObject &response)
{
    qDebug().noquote() << "\n===" << title << "===";
    qDebug().noquote() << QJsonDocument(response).toJson(QJsonDocument::Indented);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Starting C++/Qt test client...";
    qDebug() << "Connecting to Tema 8 C++/Qt server on 127.0.0.1:5555";

    QJsonObject ping;
    ping["command"] = "ping";
    printResponse("PING", sendRequest(ping));

    QJsonObject registerUser;
    registerUser["command"] = "register";
    registerUser["login"] = "admin";
    registerUser["password"] = "1234";
    printResponse("REGISTER", sendRequest(registerUser));

    QJsonObject login;
    login["command"] = "login";
    login["login"] = "admin";
    login["password"] = "1234";
    printResponse("LOGIN", sendRequest(login));

    QJsonObject sha512;
    sha512["command"] = "sha512";
    sha512["login"] = "admin";
    sha512["text"] = "hello";
    printResponse("SHA-512", sendRequest(sha512));

    QJsonObject rsa;
    rsa["command"] = "rsa";
    rsa["login"] = "admin";
    rsa["text"] = "hello";
    printResponse("RSA", sendRequest(rsa));

    QJsonObject bisection;
    bisection["command"] = "bisection";
    bisection["login"] = "admin";
    bisection["a"] = 1;
    bisection["b"] = 2;
    bisection["iterations"] = 50;
    printResponse("BISECTION", sendRequest(bisection));

    QJsonObject shortestPath;
    shortestPath["command"] = "shortest_path";
    shortestPath["login"] = "admin";
    shortestPath["from"] = 0;
    shortestPath["to"] = 4;
    printResponse("SHORTEST PATH", sendRequest(shortestPath));

    QJsonObject history;
    history["command"] = "history";
    history["limit"] = 10;
    printResponse("HISTORY", sendRequest(history));

    qDebug() << "C++/Qt test client finished.";

    return 0;
}
