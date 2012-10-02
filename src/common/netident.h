#pragma once

class CNetIdent : public QObject
{
    Q_OBJECT

public:
    CNetIdent(quint32 nStartAs);
    ~CNetIdent();
    bool Init();
    bool BroadCast();
    bool ResetBrodcast();
protected:
    bool SendBack(QUdpSocket*pSock, QHostAddress& add, quint16 nPort);
    void BuildDatagram(QByteArray& datagram);
    bool ReadDatagram(QByteArray* pDatagram, quint32& nMsg, QString& strHostName);
    bool IsLocalAddress(quint32 nIp4Add);
signals:
    void OnHostFound(QString strIpAddress, QString strHostname);
protected slots:
    void OnDatagram();
protected:
    QUdpSocket* m_pSocketArr;
    quint32     m_nNumOfSockets;
    QList<QNetworkAddressEntry> m_listBroadcastAdd;
    quint32     m_nStartAs;
    bool        m_bLocalAddressEmited;
    QString     m_strHostname;

};

