#include "QSockServer.h"
#include <QDebug>

//=========================================== ConnSocket ============================================================

QConSocket::QConSocket(qint32 socketDescriptor, QObject *parent) : QTcpSocket(parent)
{
    m_sockDesc = socketDescriptor;
    setSocketDescriptor(socketDescriptor);
    connect(this, SIGNAL(disconnected()), this, SLOT(connDisconn()));
    connect(this, SIGNAL(readyRead()), this, SLOT(connReadData()));
}

void QConSocket::connReadData()
{
    emit connReadData(this, readBufferSize());
}

void QConSocket::connDisconn()
{
    emit connDisconn(m_sockDesc);
}

//=========================================== SockServer ============================================================


QSockServer::QSockServer(QTcpServer *parent) : QTcpServer(parent)
{
    setMaxPendingConnections(16);
}

QSockServer::~QSockServer()
{
    clear();
}

bool QSockServer::listen(quint16 port)
{
    return QTcpServer::listen(QHostAddress::Any, port);
}

bool QSockServer::sendTo(qint32 sockDesc, const QByteArray &bytes)
{
    QMap<int, QConSocket*>::iterator it = m_mapClient.find(sockDesc);
    if (it != m_mapClient.end())
    {
        QConSocket* sock = it.value();
        if (sock != NULL)
        {
            sock->write(bytes, bytes.length());
            return true;
        }
    }

    return false;
}

void QSockServer::clear()
{
    // 清除
    for (QMap<int, QConSocket*>::iterator it = m_mapClient.begin();
         it != m_mapClient.end();
         ++it)
    {
        QConSocket* sock = it.value();
        if (sock != NULL)
        {
            sock->deleteLater();
        }
    }
    m_mapClient.clear();
}

void QSockServer::sockDisconn(qint32 sockDesc)
{
    m_mapClient.remove(sockDesc);
}

void QSockServer::sockReadData(QConSocket *sock, qint32 bytes)
{
    if (sock == NULL)
    {
        return;
    }

    QByteArray buff = sock->readAll();

    QString data = QString("thread id: [%1] receive from: [%2:%3] data: %4")
                .arg(QString::number(qint64(QThread::currentThreadId())))
                .arg(sock->peerAddress().toString())
                .arg(QString::number(sock->peerPort()))
                .arg(QString::fromUtf8(buff));
    emit serverReadData(data.toUtf8());
}

void QSockServer::incomingConnection(int socketDescriptor)
{
    // 达到最大连接数
    if (m_mapClient.size() >= maxPendingConnections())
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    // 调用父类方法
    QTcpServer::incomingConnection(socketDescriptor);

    // 创建新 socket
    QConSocket* sockClient = new QConSocket(socketDescriptor);
    if (sockClient == NULL)
    {
        return;
    }

    // 绑定 socket 信号
    connect(sockClient, SIGNAL(connDisconn(qint32)), this, SLOT(sockDisconn(qint32)));
    connect(sockClient, SIGNAL(connDisconn(qint32)), sockClient, SLOT(deleteLater()));
    connect(sockClient, SIGNAL(connReadData(QConSocket*, qint32)), this, SLOT(sockReadData(QConSocket*, qint32)));

    // 保存在集合
    m_mapClient.insert(socketDescriptor, sockClient);

    return;
}












