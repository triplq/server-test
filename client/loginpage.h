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

private:
    Ui::LogInPage *ui;
};

#endif // LOGINPAGE_H
