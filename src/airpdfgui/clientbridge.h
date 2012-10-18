#pragma once


#include "ibridge.h"

class CMainWnd;
class CNetSock;
class CClientBridge : public IBridgeNet
{
public:
    CClientBridge(CMainWnd* pParent);
    virtual ~CClientBridge(void);
    void  CleanUp();

    void Connect(QString strIp, quint16 nPort);
    void Disconnect();
    void SendMsg(CNetMsgBase* pMsg);
protected:
    bool OnReceivedMsg(CNetMsgBase* pMsg);
    void OnError(QString strTxt);
protected:
    CMainWnd* m_pParent;
    CNetSock* m_pNetSock;
};

