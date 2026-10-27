#ifndef USERPAGE_H
#define USERPAGE_H

#include <QWidget>
#include "usermodel.h"

namespace Ui {
class UserPage;
}

class UserPage : public QWidget
{
    Q_OBJECT

private:
    UserModel model;
    void setupTable();

public:
    explicit UserPage(QWidget *parent = nullptr);
    ~UserPage();

    void setUsers(const QVector<User>& array);

signals:
    void getClicked();

private:
    Ui::UserPage *ui;
};

#endif // USERPAGE_H
