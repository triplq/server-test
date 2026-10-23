#include "db.h"
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCoreApplication>

DB::DB(QObject* parent) : QObject(parent), connectionName("main_connection") {
}

DB::~DB() {
    disconnectDB();
}

bool DB::connectToDB() {
    if (QSqlDatabase::contains(connectionName)) {
        return true;
    }
    const QString path = QCoreApplication::applicationDirPath() + "/data/app.db";
    if (!QFile::exists(path)) {
        qCritical() << "db does not exists\n";
        return false;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(path);

    if (!db.open()) {
        qCritical() << "error open db: " << db.lastError().text();
        return false;
    }
    return true;
}

void DB::disconnectDB() {
    {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (db.isOpen() && db.isValid())
            db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
}
