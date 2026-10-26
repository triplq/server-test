#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

namespace Ui {
class LogInPage;
}

class LogInPage : public QWidget
{
    Q_OBJECT

public:
    explicit LogInPage(QWidget *parent = nullptr);
    ~LogInPage();

signals:
    void backClicked();
    void okPressed(const QString& username, const QString& email);

private:
    Ui::LogInPage *ui;
};

#endif // LOGINPAGE_H
