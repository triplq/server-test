#include "apiclient.h"
#include <QtCore>

ApiClient::ApiClient() {}

void ApiClient::connectToServer() {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ApiClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", 2323);
}

void ApiClient::slotReadyRead() {
    QDataStream in(socket);

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

    QByteArray request = QJsonDocument(answer).toJson(QJsonDocument::Compact);
    sendToServer(answer, ApiClient::requestType::existsUser);
}

void ApiClient::formInsertRequest(const QString& username, const QString& email) {
    QJsonObject answer;
    answer["action"] = "add_user";
    answer["username"] = username;
    answer["email"] = email;

    sendToServer(answer, ApiClient::requestType::insertUser);
}

void ApiClient::sendToServer(QJsonObject& request, const requestType& type) {
    QByteArray data;
    int id = nextId++;
    pending.insert(id, type);
    request["id"] = id;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << request;
    socket->write(data);
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
        emit usersGet(json["message"].toString());
        break;
    }
}