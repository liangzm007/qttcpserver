#include "QServerDlg.h"
#include "ui_qserverdlg.h"
#include <QListWidgetItem>

QServerDlg::QServerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QServerDlg)
{
    ui->setupUi(this);    

    ui->btn_stop->setEnabled(false);
    ui->btn_start->setEnabled(true);
    ui->listWidget->addItem(new QListWidgetItem("main process start, main thread id:" + QString::number(qint32(QThread::currentThreadId()))));
}

QServerDlg::~QServerDlg()
{
    delete ui;
}

void QServerDlg::on_btn_start_clicked()
{
    m_socket.listen(33333);
    connect(&m_socket, SIGNAL(serverReadData(const QByteArray&)), this, SLOT(on_server_reviced(const QByteArray&)));

    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);
}

void QServerDlg::on_btn_stop_clicked()
{
    m_socket.clear();
    m_socket.close();

    disconnect(&m_socket, SIGNAL(serverReadData(const QByteArray&)), this, SLOT(on_server_reviced(const QByteArray&)));

    ui->btn_stop->setEnabled(false);
    ui->btn_start->setEnabled(true);
}

void QServerDlg::on_server_reviced(const QByteArray& data)
{
    ui->listWidget->addItem(new QListWidgetItem(QString::fromUtf8(data)));
}
