#pragma once

#include "ibridge.h"

class CServer;
class CSrvBridge : public IBridgeNet
{
public:
    CSrvBridge(void);
    virtual ~CSrvBridge(void);
    bool OnReceivedMsg(CNetMsgBase* pMsg);    
protected:
    CServer* m_pServer;    
};

class CNetSrvBridge : public CSrvBridge
{
public:
     CNetSrvBridge(void);
    virtual ~CNetSrvBridge(void);
    void SetNetSock(CNetSock* pSock);

    void SendMsg(CNetMsgBase* pMsg);
    void OnError(QString strTxt);
protected:
    CNetSock* m_pNetSocket;
};

