#include "StdAfx.h"
#include "server.h"
#include "ibridge.h"
#include "netmsgs.h"

#include "appsettings.h"
CServer::CServer(IBridgeNet* pNet)
    :m_pNet(pNet)
{
    connect(this, SIGNAL(SendMsg(CNetMsgBase*)), this, SLOT(SendMsgSlot(CNetMsgBase*)),Qt::QueuedConnection);
    connect(this, SIGNAL(OnError(CNetMsgBase*)), this, SLOT(OnErrorSlot(CNetMsgBase*)), Qt::QueuedConnection);

     CAppSettings::Instance()->GetRoots(m_hashRoots);
}

CServer::~CServer(void)
{
}

bool CServer::OnMsgGuiVersion(CNetMsgGuiVersion*pMsg)
{
    AIRPDF_LOG(LOG_LEVEL_INFO,  QString("Gui version %1.%2 from %3").arg(pMsg->MajorVersion()).arg(pMsg->MinorVersion()).arg(pMsg->Comp()));
    emit SendMsg(new CNetMsgSrvInfo(0, ""));
    return true;
}
bool CServer::OnMsgIosVersion(CNetMsgIosVersion*pMsg)
{
    AIRPDF_LOG(LOG_LEVEL_INFO,  QString("IOS version %1.%2. Device %3 %4.").arg(pMsg->MajorVersion()).arg(pMsg->MinorVersion()).arg(pMsg->Device()).arg(pMsg->DevName()));
    emit SendMsg(new CNetMsgSrvInfo(0, ""));
    return true;
}
bool CServer::OnMsgReqDir(CNetMsgReqDir*pMsg)
{
    QString strDir = pMsg->Root();
     if (strDir.isEmpty())
     {
         return SendRootDirs();
     }
     else
     {
        if (!m_hashRoots.contains( pMsg->Root()))
        {
            return SendRootDirs();
        }

        QString strAbs = m_hashRoots[pMsg->Root()];            
        QString strDir = pMsg->RelativeDir();


     }
    return true;
}

bool  CServer::SendRootDirs()
{
    QList<QString> listRoots=m_hashRoots.keys();
    QList<CNetMsgDirFiles::CItem> list;
    while (!listRoots.isEmpty())
    {
        QString strRoot = listRoots.takeFirst();
        QString strDir = m_hashRoots[strRoot];
        QDir dir(strDir);
        if (dir.exists(strDir))
        {
            list.append(CNetMsgDirFiles::CItem(strRoot, 0, 0));
        }
    }
    emit SendMsg(new CNetMsgDirFiles(list, true)); 
    return true;
}

void CServer::SendMsgSlot(CNetMsgBase* pMsg)
{
    m_pNet->SendMsg(pMsg);
}

void CServer::OnErrorSlot(QString strTxt)
{
    m_pNet->OnError(strTxt);
}