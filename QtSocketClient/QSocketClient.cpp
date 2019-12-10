#include "QSocketClient.h"
#include "ui_QSocketClient.h"
#include <QRegExp>
#include <QDebug>
#include <QHostAddress>
#include <QListWidgetItem>

QSocketClient::QSocketClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSocketClient)
{
    ui->setupUi(this);

    connect(&m_sockClient, SIGNAL(readyRead()), this, SLOT(on_receive_data()));
}

QSocketClient::~QSocketClient()
{
    delete ui;
}

void QSocketClient::on_btn_send_clicked()
{
    if (m_sockClient.peerPort() != 33333)
    {
        QString ip   = ui->edit_ip->text();
        QString port = ui->edit_port->text();

        QRegExp exp("^((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}$");
        if (!exp.exactMatch(ip))
        {
            qDebug() << "ip error";
            return;
        }

        m_sockClient.connectToHost(QHostAddress(ip), port.toInt());
    }


    QByteArray data = ui->edit_data->text().toUtf8();
    m_sockClient.write(data, data.length());
}

void QSocketClient::on_receive_data()
{
    QByteArray data = m_sockClient.readAll();
    ui->listWidget->addItem(new QListWidgetItem(data.data()));
}
