#include "loginpage.h"
#include "ui_loginpage.h"

LogInPage::LogInPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogInPage)
{
    ui->setupUi(this);

    connect(ui->backButton, &QPushButton::clicked, this, &LogInPage::backClicked);
    connect(ui->okButton, &QPushButton::clicked, this, [this]{
        QString username = ui->usernameEdit->text();
        QString email = ui->emailEdit->text();

        ui->usernameEdit->clear();
        ui->emailEdit->clear();

        emit okPressed(username, email);
    });
}

LogInPage::~LogInPage()
{
    delete ui;
}
