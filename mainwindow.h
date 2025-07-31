#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpServer>
#include<QTcpSocket>
#include <QMainWindow>
#include<QLabel>
#include <QHostInfo>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QString  getlocalip();
private slots:
    void onConnected();
    // QTcpSocket::disconnected()
    void onDisconnected();
    // QTcpSocket::stateChanged(...)
    void onStateChanged(QAbstractSocket::SocketState socketState);
    // QTcpSocket::readyRead()
    void onReadyRead();
    void on_actconn_triggered();

    void on_actdis_triggered();

    void on_pushButton_clicked();

    void on_actclear_triggered();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpclient;
    QLabel  * labelsocketstate;

};
#endif // MAINWINDOW_H
