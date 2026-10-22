#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hellopage.h"
#include "loginpage.h"
#include "signuppage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectToServer();
    showHelloPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer() {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", 2323);
}

void MainWindow::showHelloPage() {
    HelloPage *helloPage = new HelloPage();
    ui->stackedWidget->addWidget(helloPage);
    ui->stackedWidget->setCurrentWidget(helloPage);

    connect(helloPage, &HelloPage::signupPageClicked, this, &MainWindow::showSignupPage);
    connect(helloPage, &HelloPage::loginPageClicked, this, &MainWindow::showLoginPage);
}

void MainWindow::showLoginPage() {
    LogInPage *loginPage = new LogInPage();
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::showSignupPage() {
    SignUpPage *signupPage = new SignUpPage();
    ui->stackedWidget->addWidget(signupPage);
    ui->stackedWidget->setCurrentWidget(signupPage);
}

void MainWindow::slotReadyRead() {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        qDebug() << str;
    } else {
        qDebug() << "read error...";
    }
}
