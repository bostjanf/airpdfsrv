#pragma once

#include "ibridge.h"

class CMuPdfWrap;
class CSrvBridge : public IBridgeNet
{
public:
    CSrvBridge(void);
    virtual ~CSrvBridge(void);
    bool OnReceivedMsg(CNetMsgBase* pMsg);    
protected:
    CMuPdfWrap* m_pMuPdf;
};

class CNetSrvBridge : public CSrvBridge
{
public:
     CNetSrvBridge(void);
    virtual ~CNetSrvBridge(void);
    void SetNetSock(CNetSock* pSock);
protected:
    bool SendMsg(CNetMsgBase* pMsg);
    bool OnError(QString strTxt);
protected:
    CNetSock* m_pNetSocket;
};

