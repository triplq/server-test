#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    api = new ApiClient;
    api->moveToThread(&netThread);

    helloPage = new HelloPage(this);
    ui->stackedWidget->addWidget(helloPage);
    connect(helloPage, &HelloPage::signupPageClicked, this, &MainWindow::showSignupPage);
    connect(helloPage, &HelloPage::loginPageClicked, this, &MainWindow::showLoginPage);

    loginPage = new LogInPage(this);
    ui->stackedWidget->addWidget(loginPage);
    connect(loginPage, &LogInPage::backClicked, this, &MainWindow::showHelloPage);
    connect(loginPage, &LogInPage::okPressed, api, &ApiClient::formExistsRequest);
    connect(api, &ApiClient::userExists, this, &MainWindow::showUserPage);


    signupPage = new SignUpPage(this);
    ui->stackedWidget->addWidget(signupPage);
    connect(signupPage, &SignUpPage::backClicked, this, &MainWindow::showHelloPage);
    connect(signupPage, &SignUpPage::okPressed, api, &ApiClient::formInsertRequest);
    connect(api, &ApiClient::userInsert, this, &MainWindow::showUserPage);


    userPage = new UserPage(this);
    ui->stackedWidget->addWidget(userPage);
    connect(userPage, &UserPage::getClicked, api, &ApiClient::formGetRequest);
    connect(api, &ApiClient::usersGet, this, [this](const QVector<User>& array){
        userPage->setUsers(array);
    });

    connect(&netThread, &QThread::started, api, &ApiClient::connectToServer);
    connect(&netThread, &QThread::finished, api, &QObject::deleteLater);

    showHelloPage();

    connect(api, &ApiClient::requestFail, this, [this](const QString& msg){
        QMessageBox::warning(this, "Error", msg);
    });

    netThread.start();
}

MainWindow::~MainWindow()
{
    netThread.quit();
    netThread.wait();
    delete ui;
}

void MainWindow::showHelloPage() {
    ui->stackedWidget->setCurrentWidget(helloPage);
}

void MainWindow::showLoginPage() {
    ui->stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::showSignupPage() {
    ui->stackedWidget->setCurrentWidget(signupPage);
}

void MainWindow::showUserPage(const QString& msg) {
    ui->stackedWidget->setCurrentWidget(userPage);
    ui->statusbar->showMessage(msg, 3000);
}