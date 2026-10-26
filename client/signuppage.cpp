#include "signuppage.h"
#include "ui_signuppage.h"

SignUpPage::SignUpPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignUpPage)
{
    ui->setupUi(this);

    connect(ui->backButton, &QPushButton::clicked, this, &SignUpPage::backClicked);
    connect(ui->okButton, &QPushButton::clicked, this, [this]{
        QString username = ui->usernameEdit->text();
        QString email = ui->emailEdit->text();

        ui->usernameEdit->clear();
        ui->emailEdit->clear();

        emit okPressed(username, email);
    });
}

SignUpPage::~SignUpPage()
{
    delete ui;
}
