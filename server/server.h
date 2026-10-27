#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include "db.h"

class Server : public QTcpServer {
    Q_OBJECT

private:
    DB& database;

    constexpr static quint32 kMaxFrame = 10 * 1024 * 1024;

    void sendToClient(QTcpSocket* socket, const QByteArray& answer);
    void processRequest(QTcpSocket* socket, const QByteArray& data);

    static QJsonObject ok(int id, const QString& msg = {});
    static QJsonObject fail(int id, const QString& msg = {});

    QJsonObject handleAddUser(const QJsonObject& json);
    QJsonObject handleLoginUser(const QJsonObject& json);
    QJsonObject handleGetUsers(const QJsonObject& json);

public:
    Server(DB& n_database);

public slots:
    void slotReadyRead();
    void incomingConnection(qintptr socketDescriptor);
};
