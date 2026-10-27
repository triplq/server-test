#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QTimer>
#include "user.h"

class ApiClient : public QObject
{
    Q_OBJECT
public:
    ApiClient();
    void connectToServer();

private:
    int nextId = 1;
    QTcpSocket *socket = nullptr;
    constexpr static quint32 kMaxFrame = 10 * 1024 * 1024;
    enum class requestType {insertUser, existsUser, getUsers};
    QHash<int, requestType> pending;

    void sendToServer(QJsonObject& request, const requestType& type);

    void processAnswer(const QByteArray& data);
    QVector<User> JSONparsing(const QJsonArray& array);

    QTimer reconnectTimer;
    const static int reconnectMs = 3000;
    void reconnect();

public slots:
    void slotReadyRead();

    void formInsertRequest(const QString& username, const QString& email);
    void formExistsRequest(const QString& username, const QString& email);
    void formGetRequest();

    void tryConnect();

signals:
    void requestFail(const QString& msg);
    void userExists(const QString& msg);
    void userInsert(const QString& msg);
    void usersGet(const QVector<User>& array);
};

#endif // APICLIENT_H
