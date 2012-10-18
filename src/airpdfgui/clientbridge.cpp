#include "StdAfx.h"
#include "clientbridge.h"
#include "mainwnd.h"
#include "netsock.h"


CClientBridge::CClientBridge(CMainWnd* pParent)
    :m_pParent(pParent)
{
    m_pNetSock = new CNetSock(this, pParent);
    //now m_pNetSock is owner of this class
}


CClientBridge::~CClientBridge(void)
{

}

void  CClientBridge::CleanUp()
{
    //clean up this mess
    m_pNetSock->deleteLater(); //m_pNetSock destructor will delete CClientBridge 
}

void CClientBridge::Connect(QString strIp, quint16 nPort)
{
     m_pNetSock->connectToHost(strIp, nPort);
}

void CClientBridge::Disconnect()
{
    m_pNetSock->close();
}

void CClientBridge::SendMsg(CNetMsgBase* pMsg)
{
    m_pNetSock->SendMsg(pMsg);
}

bool CClientBridge::OnReceivedMsg(CNetMsgBase* pMsg)
{
    return m_pParent->OnReceivedMsg(pMsg);
}

void CClientBridge::OnError(QString strTxt)
{
    return m_pParent->OnNetError(strTxt);
}