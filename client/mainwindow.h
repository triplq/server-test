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

public slots:
    void slotReadyRead();

private slots:
    void on_connectButton_clicked();
    void on_sendButton_clicked();
    void on_lineEdit_returnPressed();
};
#endif // MAINWINDOW_H
