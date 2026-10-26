#ifndef USERMODEL_H
#define USERMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "user.h"

class UserModel : public QAbstractTableModel {
    Q_OBJECT

private:
    QVector<User> users;
public:
    explicit UserModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {};

    void setUsers(QVector<User> n_users) {
        beginResetModel();
        users = std::move(n_users);
        endResetModel();
    }

    int rowCount(const QModelIndex& parent = {}) const override {
        return parent.isValid() ? 0 : users.size();
    }

    int columnCount(const QModelIndex& parent = {}) const override {
        return parent.isValid() ? 0 : 2;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid())
            return {};

        const User &u = users.at(index.row());
        if (role == Qt::DisplayRole) {
            switch(index.column()){
            case 0:
                return u.username;
            case 1:
                return u.email;
            }
        }
        if (role == Qt::TextAlignmentRole && index.column() == 0) {
            return Qt::AlignCenter;
        }
        return {};
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        static const QStringList names{"Имя пользователя", "Email"};
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return names.value(section);
        return {};
    }
};

#endif // USERMODEL_H
