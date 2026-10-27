#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    api.connectToServer();
    showHelloPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showHelloPage() {
    helloPage = new HelloPage(this);
    ui->stackedWidget->addWidget(helloPage);
    ui->stackedWidget->setCurrentWidget(helloPage);

    connect(helloPage, &HelloPage::signupPageClicked, this, &MainWindow::showSignupPage);
    connect(helloPage, &HelloPage::loginPageClicked, this, &MainWindow::showLoginPage);
}

void MainWindow::showLoginPage() {
    loginPage = new LogInPage(this);
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->setCurrentWidget(loginPage);

    connect(loginPage, &LogInPage::backClicked, this, &MainWindow::showHelloPage);
    connect(loginPage, &LogInPage::okPressed, &api, &ApiClient::formExistsRequest);

    connect(&api, &ApiClient::userExists, this, &MainWindow::showUserPage);
    connect(&api, &ApiClient::requestFail, this, [this](const QString& msg){
        QMessageBox::warning(this, "Error", msg);
    });
}

void MainWindow::showSignupPage() {
    signupPage = new SignUpPage(this);
    ui->stackedWidget->addWidget(signupPage);
    ui->stackedWidget->setCurrentWidget(signupPage);

    connect(signupPage, &SignUpPage::backClicked, this, &MainWindow::showHelloPage);
    connect(signupPage, &SignUpPage::okPressed, &api, &ApiClient::formInsertRequest);

    connect(&api, &ApiClient::userInsert, this, &MainWindow::showUserPage);
    connect(&api, &ApiClient::requestFail, this, [this](const QString& msg){
        QMessageBox::warning(this, "Error", msg);
    });
}

void MainWindow::showUserPage(const QString& msg) {
    userPage = new UserPage(this);
    ui->stackedWidget->addWidget(userPage);
    ui->stackedWidget->setCurrentWidget(userPage);

    connect(userPage, &UserPage::getClicked, &api, &ApiClient::formGetRequest);

    connect(&api, &ApiClient::usersGet, this, [this](const QVector<User>& array){
        userPage->setUsers(array);
    });
    connect(&api, &ApiClient::requestFail, this, [this](const QString& msg){
        QMessageBox::warning(this, "Error", msg);
    });
}