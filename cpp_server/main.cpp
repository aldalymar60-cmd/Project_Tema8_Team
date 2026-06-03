#include <QCoreApplication>
#include <QDebug>

#include "mytcpserver.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Starting Tema 8 C++/Qt server...";

    if (!Database::instance().init("tema8_server.db")) {
        qDebug() << "Database initialization failed";
        return 1;
    }

    MyTcpServer server;

    if (!server.start(5555)) {
        qDebug() << "Server start failed";
        return 1;
    }

    qDebug() << "Server is ready.";
    qDebug() << "Address: 127.0.0.1";
    qDebug() << "Port: 5555";
    qDebug() << "Waiting for JSON requests...";

    return app.exec();
}
