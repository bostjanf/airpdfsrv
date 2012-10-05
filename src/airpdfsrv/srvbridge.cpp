#include "StdAfx.h"
#include "srvbridge.h"
#include "netsock.h"
#include "server.h"
#include "netmsgs.h"


#define CHECK_SRV_STAT() if (m_pServer == NULL){OnError(QString("Unexpected msg %1").arg(pMsgIn->MsgId())); return true;}

CSrvBridge::CSrvBridge(void)
    :m_pServer(NULL)
{
}


CSrvBridge::~CSrvBridge(void)
{
    if (m_pServer)
    {
        delete m_pServer;
        m_pServer=NULL;
    }
}

bool CSrvBridge::OnReceivedMsg(CNetMsgBase* pMsgIn)
{
    switch (pMsgIn->MsgId())
    {
        case AIRPDF_MSG_ID_GUI_VERSION:
        {
            CNetMsgGuiVersion* pMsg=(CNetMsgGuiVersion*)pMsgIn;
            if (m_pServer == NULL){m_pServer=new CServer(this);}            
            return m_pServer->OnMsgGuiVersion(pMsg);
        }
        case AIRPDF_MSG_ID_IOS_VERSION:
        {
            CNetMsgIosVersion* pMsg=(CNetMsgIosVersion*)pMsgIn;
            if (m_pServer == NULL){m_pServer=new CServer(this);}            
            return m_pServer->OnMsgIosVersion(pMsg);         
        };
        case AIRPDF_MSG_ID_REQ_DIR:
        {
            CHECK_SRV_STAT();            
            return m_pServer->OnMsgReqDir((CNetMsgReqDir*)pMsgIn);
        }
   }
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


void CNetSrvBridge::OnError(QString strTxt)
{
    m_pNetSocket->deleteLater();
}

void CNetSrvBridge::SendMsg(CNetMsgBase* pMsg)
{
    m_pNetSocket->SendMsg(pMsg);
}


