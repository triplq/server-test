#include "loginpage.h"
#include "ui_loginpage.h"

LogInPage::LogInPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogInPage)
{
    ui->setupUi(this);
}

LogInPage::~LogInPage()
{
    delete ui;
}
