#ifndef DB_H
#define DB_H

#include <QObject>
#include <QVector>
#include <optional>
#include "user.h"

class DB : public QObject
{
    Q_OBJECT

public:
    DB(QObject *parent = nullptr);

    bool connectToDB();
    std::optional<bool> insert(const QString& username, const QString& email);
    std::optional<bool> exists(const QString& username, const QString& email) const;
    std::optional<QVector<User>> showUsers() const;

    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;

    ~DB();

private:
    void disconnectDB();
    const QString connectionName;

};

#endif // DB_H
