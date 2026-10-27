#include "apiclient.h"
#include <QtCore>

ApiClient::ApiClient() {
    reconnectTimer.setSingleShot(true);
    reconnectTimer.setInterval(reconnectMs);

    connect(&reconnectTimer, &QTimer::timeout, this, &ApiClient::tryConnect);
}

void ApiClient::connectToServer() {
    if (socket)
        return;

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ApiClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [this]{
        pending.clear();
        emit requestFail("Connection to server lost");
        reconnect();
    });
    connect(socket, &QTcpSocket::errorOccurred, this,
            [this](QAbstractSocket::SocketError) {
                qWarning().noquote() << "Socket error:" << socket->errorString();
                reconnect();
            });

    connect(socket, &QTcpSocket::connected, this, [this] {
        reconnectTimer.stop();
    });

    socket->connectToHost("127.0.0.1", 2323);
}

void ApiClient::reconnect() {
    if (!reconnectTimer.isActive())
        reconnectTimer.start();
}

void ApiClient::tryConnect() {
    switch (socket->state()) {
    case QAbstractSocket::UnconnectedState:
        socket->connectToHost("127.0.0.1", 2323);
        break;
    case QAbstractSocket::ConnectedState:
        break;
    default:
        reconnect();
    }
}

void ApiClient::slotReadyRead() {
    while(socket->bytesAvailable() >= 4) {
        char raw_len[4];
        socket->peek(raw_len, 4);
        const quint32 len = qFromBigEndian<quint32>(raw_len);
        if (len > kMaxFrame) {
            qWarning() << "Big data something with server\n";
            socket->disconnectFromHost();
            return;
        }

        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);
        in.startTransaction();
        QByteArray data;
        in >> data;
        if (!in.commitTransaction())
            return;

        processAnswer(data);
    }
}

void ApiClient::formExistsRequest(const QString& username, const QString& email) {
    QJsonObject answer;
    answer["action"] = "login_user";
    answer["username"] = username;
    answer["email"] = email;

    sendToServer(answer, ApiClient::requestType::existsUser);
}

void ApiClient::formInsertRequest(const QString& username, const QString& email) {
    QJsonObject answer;
    answer["action"] = "add_user";
    answer["username"] = username;
    answer["email"] = email;

    sendToServer(answer, ApiClient::requestType::insertUser);
}

void ApiClient::formGetRequest() {
    QJsonObject answer;
    answer["action"] = "get_users";

    sendToServer(answer, ApiClient::requestType::getUsers);
}

void ApiClient::sendToServer(QJsonObject& request, const requestType& type) {
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) {
        emit requestFail("No connection to server");
        return;
    }

    QByteArray data;
    int id = nextId++;
    pending.insert(id, type);
    request["id"] = id;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << QJsonDocument(request).toJson(QJsonDocument::Compact);
    socket->write(data);
}

QVector<User> ApiClient::JSONparsing(const QJsonArray& array){
    QVector<User> users;
    for (const QJsonValue& a : array) {
        users.push_back(User::fromJson(a.toObject()));
    }

    return users;
}

void ApiClient::processAnswer(const QByteArray& data) {
    QJsonObject json = QJsonDocument::fromJson(data).object();
    int id = json["id"].toInt(-1);
    if (!pending.contains(id)) {
        return;
    }
    const requestType type = pending.take(id);
    if (json["status"].toString() == "failure") {
        emit requestFail(json["message"].toString());
        return;
    }
    switch(type) {
    case requestType::existsUser:
        emit userExists(json["message"].toString());
        break;
    case requestType::insertUser:
        emit userInsert(json["message"].toString());
        break;
    case requestType::getUsers:
        QVector<User> users = JSONparsing(json["message"].toArray());
        emit usersGet(users);
        break;
    }
}