#include "server.h"
#include "db.h"

#include <QCoreApplication>

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

    return a.exec();
}
