#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyTcpServer(QObject *parent = nullptr);

    bool start(quint16 port);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QJsonObject processRequest(const QJsonObject &request);
    QJsonObject makeError(const QString &message) const;
    QJsonObject makeOk() const;
};

#endif
