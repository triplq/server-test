#ifndef DB_H
#define DB_H

#include <QObject>

class DB : public QObject
{
    Q_OBJECT

public:
    DB(QObject *parent = nullptr);
    bool connectToDB();

    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;

    ~DB();

private:
    void disconnectDB();
    const QString connectionName;

};

#endif // DB_H
