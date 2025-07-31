#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labelsocketstate=new QLabel("socket状态");
    labelsocketstate->setMinimumWidth(300);
    ui->statusBar->addWidget(labelsocketstate);
    QString localip=getlocalip();
    setWindowTitle(windowTitle()+"本机ip"+localip);
    ui->comboBox->addItem(localip);
    tcpclient=new QTcpSocket(this);
    connect(tcpclient,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpclient,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(tcpclient,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(tcpclient,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    ui->actconn->setEnabled(true);
    ui->actdis->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}
QString     MainWindow::getlocalip(){
    QString localname=QHostInfo::localHostName();
    QHostInfo hostinfo=QHostInfo::fromName(localname);
    QString localip="";
    QList<QHostAddress>addrlist=hostinfo.addresses();
    if(!addrlist.isEmpty()){
        for(int i=0;i<addrlist.count();i++){
            QHostAddress addr=addrlist.at(i);
            if(addr.protocol()==QAbstractSocket::IPv4Protocol){
                localip=addr.toString();
                break;
            }
        }
    }
    return  localip;
}
void MainWindow::on_actconn_triggered()
{
    QString ip=ui->comboBox->currentText();
    quint16 port=ui->spinBox->value();
    tcpclient->connectToHost(ip,port);
}
void MainWindow::onConnected(){
    ui->plainTextEdit->appendPlainText("已连接服务器");
    ui->plainTextEdit->appendPlainText("peer addres"+tcpclient->peerAddress().toString());
    ui->plainTextEdit->appendPlainText("peer port"+QString::number(tcpclient->peerPort()));
    ui->actconn->setEnabled(false);
    ui->actdis->setEnabled(true);
}
void MainWindow::on_actdis_triggered()
{
    if(tcpclient->state()==QAbstractSocket::ConnectedState)
        tcpclient->disconnectFromHost();
}
void MainWindow::onDisconnected(){
    ui->plainTextEdit->appendPlainText("已断开链接");
    ui->actconn->setEnabled(true);
    ui->actdis->setEnabled(false);
}
void MainWindow::onStateChanged(QAbstractSocket::SocketState socketState){
    QMetaEnum metaenum=QMetaEnum::fromType<QAbstractSocket::SocketState>();
    labelsocketstate->setText("cocket" +QString(metaenum.valueToKey(socketState)));
}



void MainWindow::on_pushButton_clicked()
{
    QString msg=ui->lineEdit_2->text();
    ui->plainTextEdit->appendPlainText("发送"+msg);
    ui->plainTextEdit->clear();
    ui->lineEdit_2->setFocus();
    QByteArray str=msg.toUtf8();
    str.append("\n");
    tcpclient->write(str);
}
void MainWindow::onReadyRead(){
    while(tcpclient->canReadLine())
        ui->plainTextEdit->appendPlainText("接收"+tcpclient->readLine());
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(tcpclient->state()==QAbstractSocket::ConnectedState)
        tcpclient->disconnectFromHost();
    event->accept();
}




void MainWindow::on_actclear_triggered()
{
    ui->plainTextEdit->clear();
}

