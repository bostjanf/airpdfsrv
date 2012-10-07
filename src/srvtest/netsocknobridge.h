#pragma once
#include "StdAfx.h"

#include "netsock.h"
#include "ibridge.h"


class CNetSockNoBridge : public CNetSock
{
    Q_OBJECT
public:
    CNetSockNoBridge(QObject *parent=NULL);
    ~CNetSockNoBridge();
signals:
    bool OnReceivedMsg(CNetMsgBase* pMsg);
    void OnError(QString strTxt);    
protected:
    void EmitOnReceivedMsg(CNetMsgBase* pMsg);
    void EmitOnError(QString strTxt);

    //inner class to wrap to Qt signals
    class CBridgeNet : public IBridgeNet
    {
    public:
        CBridgeNet(CNetSockNoBridge* pParent);
        virtual ~CBridgeNet(void);
        bool OnReceivedMsg(CNetMsgBase* pMsg);
        void OnError(QString strTxt);
        void SendMsg(CNetMsgBase* pMsg);
    protected:
        CNetSockNoBridge* m_pParent;
    };
    friend class CBridgeNet;
};
