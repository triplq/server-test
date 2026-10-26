#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QTcpSocket>

class ApiClient : public QObject
{
    Q_OBJECT
public:
    ApiClient();
    void connectToServer();

private:
    int nextId = 1;
    QTcpSocket *socket;
    constexpr static quint32 kMaxFrame = 10 * 1024 * 1024;
    enum class requestType {insertUser, existsUser, getUsers};
    QHash<int, requestType> pending;

    void sendToServer(QJsonObject& request, const requestType& type);

    void processAnswer(const QByteArray& data);

public slots:
    void slotReadyRead();

    void formInsertRequest(const QString& username, const QString& email);
    void formExistsRequest(const QString& username, const QString& email);

signals:
    void requestFail(const QString& msg);
    void userExists(const QString& msg);
    void userInsert(const QString& msg);
    void usersGet(const QString& msg);
};

#endif // APICLIENT_H
