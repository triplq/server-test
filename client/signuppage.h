#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QWidget>

namespace Ui {
class SignUpPage;
}

class SignUpPage : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpPage(QWidget *parent = nullptr);
    ~SignUpPage();

signals:
    void backClicked();
    void okPressed(const QString& username, const QString& email);

private:
    Ui::SignUpPage *ui;
};

#endif // SIGNUPPAGE_H
