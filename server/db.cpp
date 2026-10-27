#include "db.h"
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCoreApplication>
#include <QSqlQuery>

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

std::optional<bool> DB::insert(const QString& username, const QString& email) {
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery q(db);
    q.prepare("INSERT INTO users(username, email) VALUES(:username, :email)");
    q.bindValue(":username", username);
    q.bindValue(":email", email);

    if (q.exec())
        return true;

    QSqlError error = q.lastError();
    if (error.nativeErrorCode() == "19") {
        if (error.text().contains("UNIQUE constraint failed"))
            return false;
    }

    qWarning() << "error with sql " << error.text();
    return std::nullopt;
}

std::optional<bool> DB::exists(const QString& username, const QString& email) const {
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery q(db);
    q.prepare("SELECT 1 FROM users WHERE username = :username AND email = :email LIMIT 1");
    q.bindValue(":username", username);
    q.bindValue(":email", email);
    if (!q.exec()) {
        qWarning() << "error with sql " << q.lastError().text();
        return std::nullopt;
    }

    return q.next();
}

std::optional<QVector<User>> DB::showUsers() const {
    QVector<User> users;
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QSqlQuery q(db);
    q.prepare("SELECT username, email FROM users");
    if (q.exec()) {
        while (q.next()) {
            User user;
            user.username = q.value("username").toString();
            user.email = q.value("email").toString();
            users.push_back(user);
        }
    } else {
        qWarning() << "error with sql " << q.lastError().text();
        return std::nullopt;
    }

    return users;
}