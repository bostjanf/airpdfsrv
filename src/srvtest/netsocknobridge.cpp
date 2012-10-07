#include "StdAfx.h"
#include "netsocknobridge.h"

CNetSockNoBridge::CNetSockNoBridge(QObject *parent)
    :CNetSock(new CBridgeNet(this), parent)
{
}

CNetSockNoBridge::~CNetSockNoBridge()
{

}

void CNetSockNoBridge::EmitOnReceivedMsg(CNetMsgBase* pMsg)
{
    emit OnReceivedMsg(pMsg);
}

void CNetSockNoBridge::EmitOnError(QString strTxt)
{
    emit OnError(strTxt);
}

/////////////////////CNetSockNoBridge::CBridgeNet
CNetSockNoBridge::CBridgeNet::CBridgeNet(CNetSockNoBridge* pParent)
    :m_pParent(pParent)
{
}

CNetSockNoBridge::CBridgeNet::~CBridgeNet(void)
{

}

bool CNetSockNoBridge::CBridgeNet::OnReceivedMsg(CNetMsgBase* pMsg)
{
    Q_ASSERT(m_pParent);
    if (m_pParent)
    {
        m_pParent->EmitOnReceivedMsg(pMsg);
    }
    return true;
}

void CNetSockNoBridge::CBridgeNet::OnError(QString strTxt)
{
    Q_ASSERT(m_pParent);
    if (m_pParent)
    {
        m_pParent->EmitOnError(strTxt);
    }
}

void CNetSockNoBridge::CBridgeNet::SendMsg(CNetMsgBase*)
{
    Q_ASSERT(NULL);
}
