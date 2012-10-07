#pragma once


class CNetSockNoBridge;
class CTestVersions : public QObject
{
    Q_OBJECT
public:
    CTestVersions(CNetSockNoBridge*pNet, QString strIP, bool &bTestFailed);
    ~CTestVersions();
public slots:
    bool OnReceivedMsg(CNetMsgBase* pMsg);
    void OnError(QString strTxt);    
private slots:
    void TestConnect();
    void TestVersion();
protected:
    void WaitForMsg(quint32 nMsg);
    void OnMsg(CNetMsgBase* pMsgIn);
private:
    CNetSockNoBridge*m_pNet;
    QString m_strIP;
    quint32 m_nMsgGet;
    bool& m_bTestFailed;
};

