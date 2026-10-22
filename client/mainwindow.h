#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QTcpSocket *socket;
    QByteArray data;
    Ui::MainWindow *ui;
    void sendToServer(QString str);

    void showHelloPage();
    void showLoginPage();
    void showSignupPage();
    void showHomePage();

    void connectToServer();

public slots:
    void slotReadyRead();

};
#endif // MAINWINDOW_H
