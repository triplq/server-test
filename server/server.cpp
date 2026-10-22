#include "server.h"

Server::Server() {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server listens\n";
    } else {
        qDebug() << "Server is not listening\n";
    }
};

void Server::incommingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.push_back(socket);
    qDebug() << "connect" << socketDescriptor << '\n';
}

void Server::slotReadyRead() {
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);
    if (in.status() == QDataStream::Ok) {
        qDebug() << "read...\n";
        QString str;
        in >> str;
        qDebug() << str << '\n';
    } else {
        qDebug() << "DataStream error\n";
    }
}

void Server::sendToClient(QString str) {
    data.clear();
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << str;
    socket->write(data);
}