#ifndef QSERVERDLG_H
#define QSERVERDLG_H

#include <QDialog>
#include "QSockServer.h"

namespace Ui {
class QServerDlg;
}

class QServerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QServerDlg(QWidget *parent = 0);
    ~QServerDlg();

private slots:
    void on_btn_start_clicked();
    void on_btn_stop_clicked();
    void on_server_reviced(const QByteArray&);

private:
    Ui::QServerDlg *ui;

    QSockServer m_socket;
};

#endif // QSERVERDLG_H
