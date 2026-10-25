#include "server.h"
#include <QtEndian>
#include <QtCore>

Server::Server(DB& n_database) : database(n_database) {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server listens\n";
    } else {
        qDebug() << "Server is not listening\n";
    }
};

void Server::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.push_back(socket);
    qDebug() << "connect" << socketDescriptor << '\n';
}

void Server::slotReadyRead() {
    socket = qobject_cast<QTcpSocket*>(sender());

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

void Server::processRequest(QTcpSocket* socket, const QByteArray& data) {
    const QJsonObject json = QJsonDocument::fromJson(data).object();
    const QString action = json["action"].toString();

    QJsonObject answer;

    if (action == "add_user") {
        QString username = json["username"].toString();
        QString email = json["email"].toString();
        if (!username.isEmpty() && !email.isEmpty()) {
            auto insert_result = database.insert(username, email);
            if (insert_result) {
                answer["status"] = "success";
                answer["message"] = "User added successfully";
            } else {
                answer["status"] = "failure";
                answer["message"] = "Failed to execute sql query";
            }
        } else {
            answer["status"] = "failure";
            answer["message"] = "Empty fields username email";  //добавить вывод конктретики - тянуть из database->insert текст ошибки
        }
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