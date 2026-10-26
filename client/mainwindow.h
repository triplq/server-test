#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include "apiclient.h"
#include "userpage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;    

private:
    Ui::MainWindow *ui;
    ApiClient api;

    UserPage* userPage;

    void showHelloPage();
    void showLoginPage();
    void showSignupPage();
    void showHomePage();
    void showUserPage(const QString& msg);
    void showUsers(const QVector<User>& array);

};
#endif // MAINWINDOW_H
