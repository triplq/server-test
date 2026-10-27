#include "server.h"
#include <QtEndian>
#include <QtCore>
#include <QVector>
#include "user.h"

Server::Server(DB& n_database) : database(n_database) {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server listens\n";
    } else {
        qDebug() << "Server is not listening\n";
    }
};

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    qDebug() << "connect" << socketDescriptor << '\n';
}

void Server::slotReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if (!socket)
        return;

    while(socket->bytesAvailable() >= 4) {
        char raw_len[4];
        socket->peek(raw_len, 4);
        const quint32 len = qFromBigEndian<quint32>(raw_len);
        if (len > kMaxFrame) {
            qWarning() << "Big data mb attack\n";
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

        processRequest(socket, data);
    }
}

QJsonObject Server::ok(int id, const QString& msg) {
    return { {"status", "success"}, {"message", msg}, {"id", id}};
}

QJsonObject Server::fail(int id, const QString& msg) {
    return { {"status", "failure"}, {"message", msg}, {"id", id}};
}

QJsonObject Server::handleAddUser(const QJsonObject& json) {
    QString username = json["username"].toString();
    QString email = json["email"].toString();
    int id = json["id"].toInt();

    if (username.isEmpty() || email.isEmpty()) {
        return fail(id ,"Empty fields username email");
    }

    auto insert_result = database.insert(username, email);
    if (!insert_result) {
        return fail(id, "Failed to execute sql");
    } else if (*insert_result == true) {
        return ok(id, "User added successfully");
    } else {
        return fail(id, "User does exists");
    }
}

QJsonObject Server::handleLoginUser(const QJsonObject& json) {
    QString username = json["username"].toString();
    QString email = json["email"].toString();
    int id = json["id"].toInt();

    if (username.isEmpty() || email.isEmpty()) {
        return fail(id, "Empty fields username email");
    }

    auto exists_result = database.exists(username, email);
    if (!exists_result) {
        return fail(id, "Failed to execute sql");
    } else if (*exists_result == true) {
        return ok(id, "User login successfully");
    } else {
        return fail(id, "User does not exists");
    }
}

QJsonObject Server::handleGetUsers(const QJsonObject& json) {
    auto showUsers_result = database.showUsers();
    int id = json["id"].toInt();
    if (showUsers_result) {
        QJsonArray list;
        QVector<User> userList = *showUsers_result;
        for (const auto& u : userList) {
            list.append(QJsonObject{
                {"username", u.username},
                {"email", u.email}
            });
        }
        QJsonObject answer = ok(id);
        answer["message"] = list;
        return answer;
    } else {
        return fail(id, "Error with sql");
    }
}

void Server::processRequest(QTcpSocket* socket, const QByteArray& data) {
    const QJsonObject json = QJsonDocument::fromJson(data).object();
    const QString action = json["action"].toString();
    int id = json["id"].toInt();

    QJsonObject answer;

    if (action == "add_user") {
        answer = handleAddUser(json);
    } else if (action == "login_user") {
        answer = handleLoginUser(json);
    } else if (action == "get_users") {
        answer = handleGetUsers(json);
    } else {

        answer = fail(id, "No action");
    }

    QByteArray bytes_answer = QJsonDocument(answer).toJson(QJsonDocument::Compact);
    sendToClient(socket, bytes_answer);
}

void Server::sendToClient(QTcpSocket* socket, const QByteArray& answer) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << answer;
    socket->write(data);
}