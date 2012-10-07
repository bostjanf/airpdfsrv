#pragma once

class CNetMsgBase;
class IBridgeNet;
class CNetSock : public QTcpSocket
{
    Q_OBJECT
public:
    CNetSock(IBridgeNet* m_pBridge, QObject *parent=NULL);
    virtual ~CNetSock();
    void SendMsg(CNetMsgBase*);
    IBridgeNet* Bridge();
protected:
    void ForceClose(QString strError);    
protected slots:
    void SockBytesSend(qint64);
    void SockReadyRead();
    void SockError(QAbstractSocket::SocketError);
protected:
    CNetMsgBase* m_pMsgOut;
    CNetMsgBase* m_pMsgIn;
    QList<CNetMsgBase*> m_listOut;
    QByteArray m_arrLeftOver;

    IBridgeNet* m_pBridge;
};