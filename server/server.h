#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class Server : public QTcpServer {
    Q_OBJECT

private:
    QVector <QTcpSocket*> sockets;
    QByteArray data;
    void sendToClient(QString str);

public:
    Server();
    QTcpSocket *socket;

public slots:
    void slotReadyRead();
    void incomingConnection(qintptr socketDescriptor);
};
