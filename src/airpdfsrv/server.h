#pragma once

class IBridgeNet;

class CNetMsgGuiVersion;
class CNetMsgIosVersion;
class CNetMsgReqDir;

class CServer : public QObject
{
    Q_OBJECT
public:
    CServer(IBridgeNet* pNet);
    virtual ~CServer(void);

    virtual bool OnMsgGuiVersion(CNetMsgGuiVersion*);
    virtual bool OnMsgIosVersion(CNetMsgIosVersion*);
    virtual bool OnMsgReqDir(CNetMsgReqDir*);
signals:
    void SendMsg(CNetMsgBase* pMsg);
    void OnError(QString strTxt);
protected slots:
    void SendMsgSlot(CNetMsgBase* pMsg);
    void OnErrorSlot(QString strTxt);
protected:
    bool  SendRootDirs();
protected:
    QHash<QString, QString> m_hashRoots;   
private:
    IBridgeNet* m_pNet;
};

