#include "hellopage.h"
#include "ui_hellopage.h"

HelloPage::HelloPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HelloPage)
{
    ui->setupUi(this);

    connect(ui->signupButton, &QPushButton::clicked, this, &HelloPage::signupPageClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &HelloPage::loginPageClicked);
}

HelloPage::~HelloPage()
{
    delete ui;
}
