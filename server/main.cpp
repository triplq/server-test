#include "server.h"
#include "db.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DB check("startup");
    if(!check.connectToDB()){
        qCritical() << "error to connect db\n";
        return 1;
    } else {
        qDebug() << "db connected succes\n";
    }

    Server server;
    if (!server.isListening()) {
        qCritical() << "server is not listening\n";
        return 1;
    }

    return a.exec();
}
