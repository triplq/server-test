#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include "db.h"

class Server : public QTcpServer {
    Q_OBJECT

private:
    DB& database;

    QVector <QTcpSocket*> sockets;
    constexpr static quint32 kMaxFrame = 10 * 1024 * 1024;

    void sendToClient(QTcpSocket* socket, const QByteArray& answer);
    void processRequest(QTcpSocket* socket, const QByteArray& data);

public:
    Server(DB& n_database);
    QTcpSocket *socket;

public slots:
    void slotReadyRead();
    void incomingConnection(qintptr socketDescriptor);
};
