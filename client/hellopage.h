#ifndef HELLOPAGE_H
#define HELLOPAGE_H

#include <QWidget>

namespace Ui {
class HelloPage;
}

class HelloPage : public QWidget
{
    Q_OBJECT

public:
    explicit HelloPage(QWidget *parent = nullptr);
    ~HelloPage();

private:
    Ui::HelloPage *ui;

signals:
    void signupPageClicked();
    void loginPageClicked();
};

#endif // HELLOPAGE_H
