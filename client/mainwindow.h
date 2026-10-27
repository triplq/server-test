#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include "apiclient.h"
#include "hellopage.h"
#include "loginpage.h"
#include "signuppage.h"
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
    LogInPage* loginPage;
    SignUpPage* signupPage;
    HelloPage* helloPage;

    void showHelloPage();
    void showLoginPage();
    void showSignupPage();
    void showUserPage(const QString& msg);

};
#endif // MAINWINDOW_H
