#include "server.h"
#include <QtEndian>
#include <QtCore>
#include <QVector>
#include <QThreadPool>
#include <QPointer>
#include <QUuid>
#include "user.h"

Server::Server() {
    QThreadPool::globalInstance()->setMaxThreadCount(64);
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server listens\n";
    } else {
        qDebug() << "Server is not listening\n";
    }
};

Server::~Server() {
    QThreadPool::globalInstance()->waitForDone();
}

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

        buildReply(socket, data);
    }
}

QJsonObject Server::ok(int id, const QString& msg) {
    return { {"status", "success"}, {"message", msg}, {"id", id}};
}

QJsonObject Server::fail(int id, const QString& msg) {
    return { {"status", "failure"}, {"message", msg}, {"id", id}};
}

QJsonObject Server::handleAddUser(DB& db, const QJsonObject& json) {
    QString username = json["username"].toString();
    QString email = json["email"].toString();
    int id = json["id"].toInt();

    if (username.isEmpty() || email.isEmpty()) {
        return fail(id ,"Empty fields username email");
    }

    auto insert_result = db.insert(username, email);
    if (!insert_result) {
        return fail(id, "Failed to execute sql");
    } else if (*insert_result == true) {
        return ok(id, "User added successfully");
    } else {
        return fail(id, "User does exists");
    }
}

QJsonObject Server::handleLoginUser(DB& db, const QJsonObject& json) {
    QString username = json["username"].toString();
    QString email = json["email"].toString();
    int id = json["id"].toInt();

    if (username.isEmpty() || email.isEmpty()) {
        return fail(id, "Empty fields username email");
    }

    auto exists_result = db.exists(username, email);
    if (!exists_result) {
        return fail(id, "Failed to execute sql");
    } else if (*exists_result == true) {
        return ok(id, "User login successfully");
    } else {
        return fail(id, "User does not exists");
    }
}

QJsonObject Server::handleGetUsers(DB& db, const QJsonObject& json) {
    auto showUsers_result = db.showUsers();
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

QByteArray Server::buildReply(QTcpSocket* socket, const QByteArray& data) {
    const QJsonObject json = QJsonDocument::fromJson(data).object();
    const QString action = json["action"].toString();
    int id = json["id"].toInt();

    QJsonObject answer;
    DB database(QUuid::createUuid().toString());

    if (action == "add_user") {
        answer = handleAddUser(database, json);
    } else if (action == "login_user") {
        answer = handleLoginUser(database, json);
    } else if (action == "get_users") {
        answer = handleGetUsers(database, json);
    } else {
        answer = fail(id, "No action");
    }

    QByteArray bytes_answer = QJsonDocument(answer).toJson(QJsonDocument::Compact);
    return bytes_answer;
}

void Server::processRequest(QTcpSocket* socket, const QByteArray& data) {
    QPointer<QTcpSocket> guard(socket);
    QThreadPool::globalInstance()->start([this, guard, data]{
        QByteArray reply = buildReply(guard, data);
        QMetaObject::invokeMethod(this, [this, guard, reply]{
            if (guard) {
                sendToClient(guard, reply);
            }
        }, Qt::QueuedConnection);
    });
}

void Server::sendToClient(QTcpSocket* socket, const QByteArray& answer) {
    if (!socket || socket->state() != QAbstractSocket::ConnectedState)
        return;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << answer;
    socket->write(data);
}