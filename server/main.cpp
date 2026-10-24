#include "server.h"
#include "db.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DB database;

    if(!database.connectToDB()){
        qCritical() << "error to connect db\n";
        return 1;
    } else {
        qDebug() << "db connected succes\n";
    }

    auto found_ins = database.insert("Alexus", "qwer@mail");
    if (!found_ins) {
        qDebug() << "error with insert\n";
    } else {
        qDebug() << *found_ins << '\n';
    }
    auto found = database.exists("Alexus", "qwer@mail");
    if (!found) {
        qDebug() << "error with exists\n";
    } else {
        qDebug() << found.value() << " here this" << '\n';
    }

    // auto found_vec = database.showUsers();
    // if (!found_vec.has_value()) {
    //     qDebug() << "error with show users\n";
    // } else {
    //     for (auto& v : found_vec.value())
    //         qDebug() << v.username << ' ' << v.email << '\n';
    // }

    Server server;

    return a.exec();
}
