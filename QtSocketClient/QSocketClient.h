#ifndef QSOCKETCLIENT_H
#define QSOCKETCLIENT_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class QSocketClient;
}

class QSocketClient : public QDialog
{
    Q_OBJECT

public:
    explicit QSocketClient(QWidget *parent = 0);
    ~QSocketClient();

private slots:
    void on_btn_send_clicked();
    void on_receive_data();

private:
    Ui::QSocketClient *ui;

    QTcpSocket m_sockClient;
};

#endif // QSOCKETCLIENT_H
