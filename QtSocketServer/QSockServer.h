#ifndef QSOCKSERVER_H
#define QSOCKSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QThread>

namespace WhInitClient {
    class QConSocket;
    class QSockServer;
}

class QConSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit QConSocket(qint32 socketDescriptor, QObject* parent = 0);

public slots:
    void connDisconn();
    void connReadData();

signals:
    void connDisconn(qint32 sockDesc);
    void connReadData(QConSocket* sock, qint32 bytes);

protected:
    qint32 m_sockDesc;
};

class QSockServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit QSockServer(QTcpServer *parent = 0);
    ~QSockServer();

    bool listen(quint16 port);
    bool sendTo(qint32 sockDesc, const QByteArray& bytes);
    void clear();

public slots:
    void sockDisconn(qint32 sockDesc);
    void sockReadData(QConSocket* sock, qint32 bytes);

signals:
    void serverReadData(const QByteArray& bytes);

protected:
    virtual void incomingConnection (int socketDescriptor);

    QMap<int, QConSocket*> m_mapClient;
};

#endif // QSOCKSERVER_H
