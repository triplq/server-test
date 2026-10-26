#include "userpage.h"
#include "ui_userpage.h"

UserPage::UserPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserPage)
{
    ui->setupUi(this);
    setupTable();
    connect(ui->getButton, &QPushButton::clicked, this, &UserPage::getClicked);
}

UserPage::~UserPage()
{
    delete ui;
}

void UserPage::setUsers(const QVector<User>& array) {
    model.setUsers(array);
}

void UserPage::setupTable()
{
    proxy.setSourceModel(&model);
    ui->tableView->setModel(&proxy);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->verticalHeader()->setVisible(false);

    auto *h = ui->tableView->horizontalHeader();
    h->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(2, QHeaderView::Stretch);
}