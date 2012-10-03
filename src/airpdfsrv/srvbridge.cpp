#include "StdAfx.h"
#include "srvbridge.h"
#include "mupdfwrap.h"
#include "netsock.h"

#include "netmsgs.h"

CSrvBridge::CSrvBridge(void)
{
}


CSrvBridge::~CSrvBridge(void)
{
}


bool CSrvBridge::OnReceivedMsg(CNetMsgBase* pMsg)
{
    return true;
}


///////////////////////////////////////

CNetSrvBridge::CNetSrvBridge(void)
    :m_pNetSocket(NULL)
{
}


CNetSrvBridge::~CNetSrvBridge(void)
{
}


void CNetSrvBridge::SetNetSock(CNetSock* pSock)
{
    m_pNetSocket=pSock;
    SendMsg(new CNetMsgSrvVersion());
}


bool CNetSrvBridge::OnError(QString strTxt)
{
    m_pNetSocket->deleteLater();
    return true;
}

bool CNetSrvBridge::SendMsg(CNetMsgBase* pMsg)
{
    m_pNetSocket->SendMsg(pMsg);
    return true;
}


