#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include "db.h"

class Server : public QTcpServer {
    Q_OBJECT

private:
    constexpr static quint32 kMaxFrame = 10 * 1024 * 1024;

    void sendToClient(QTcpSocket* socket, const QByteArray& answer);
    QByteArray buildReply(QTcpSocket* socket, const QByteArray& data);
    void processRequest(QTcpSocket* socket, const QByteArray& data);

    static QJsonObject ok(int id, const QString& msg = {});
    static QJsonObject fail(int id, const QString& msg = {});

    QJsonObject handleAddUser(DB& db, const QJsonObject& json);
    QJsonObject handleLoginUser(DB& db, const QJsonObject& json);
    QJsonObject handleGetUsers(DB& db, const QJsonObject& json);

public:
    Server();
    ~Server();

public slots:
    void slotReadyRead();
    void incomingConnection(qintptr socketDescriptor);
};
