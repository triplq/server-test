#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

struct User {
    QString username;
    QString email;

    static User fromJson(const QJsonObject& o) {
        return {o["username"].toString(), o["email"].toString()};
    }
};

#endif // USER_H
