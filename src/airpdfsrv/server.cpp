#include "StdAfx.h"
#include "server.h"
#include "ibridge.h"
#include "netmsgs.h"

#include "appsettings.h"
CServer::CServer(IBridgeNet* pNet)
    :m_pNet(pNet)
{
    connect(this, SIGNAL(SendMsg(CNetMsgBase*)), this, SLOT(SendMsgSlot(CNetMsgBase*)),Qt::QueuedConnection);
    connect(this, SIGNAL(OnError(QString)), this, SLOT(OnErrorSlot(QString)), Qt::QueuedConnection);

    CAppSettings::Instance()->GetRoots(m_hashRoots);
}

CServer::~CServer(void)
{
}

bool CServer::OnMsgGuiVersion(CNetMsgGuiVersion*pMsg)
{
    AIRPDF_LOG(LOG_LEVEL_INFO,  QString("Gui version %1.%2 from %3").arg(pMsg->MajorVersion()).arg(pMsg->MinorVersion()).arg(pMsg->Comp()));
    SendSrvInfo();
    return true;
}

bool CServer::OnMsgIosVersion(CNetMsgIosVersion*pMsg)
{
    AIRPDF_LOG(LOG_LEVEL_INFO,  QString("IOS version %1.%2. Device %3 %4.").arg(pMsg->MajorVersion()).arg(pMsg->MinorVersion()).arg(pMsg->Device()).arg(pMsg->DevName()));
    SendSrvInfo();
    return true;
}


bool CServer::OnMsgReqDir(CNetMsgReqDir*pMsg)
{
    QString stRoot = pMsg->Root();
     if (stRoot.isEmpty())
     {
         return SendRootDirs();
     }
     
    if (!m_hashRoots.contains(stRoot))
    {
        return SendRootDirs();
    }

    QString strAbs = m_hashRoots[stRoot];            
    QString strDir = pMsg->RelativeDir();
        if (!strDir.isEmpty())
    {
        strAbs.append(QDir::separator());
#ifdef QT_DEBUG
        if (strDir.endsWith("\\") ||
            strDir.endsWith("/"))            
        {
            Q_ASSERT(NULL);
        }
#endif
        strAbs.append(strDir);
    }

    strAbs.remove("..");
    QDir dir(strAbs);
    if (!dir.exists())
    {
        return SendRootDirs();
    }

    QList<CNetMsgDirFiles::CItem> listItems;
    {//dirs first
                    
        QStringList list = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name);
        while (!list.isEmpty())
        {                        
            listItems.append(CNetMsgDirFiles::CItem(list.takeFirst(), 0, 0));
        }
    }

    QStringList list = dir.entryList(QDir::Files, QDir::Name);
    while (!list.isEmpty())
    {
        QString strFile = list.takeFirst();
        if (strFile.endsWith(".pdf", Qt::CaseInsensitive))
        {
            QFileInfo info(dir.absoluteFilePath(strFile));
            listItems.append(CNetMsgDirFiles::CItem(strFile, 1, info.size()));
        }
    }
    emit SendMsg(new CNetMsgDirFiles(listItems, false));
    return false;
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

void  CServer::SendSrvInfo()
{
    quint32 nFlags=0;
    if (m_hashRoots.isEmpty())
    {
        nFlags |=SRV_INFO_FLAG_NO_ROOTS;
    }
    if (!CAppSettings::Instance()->GetPwd().isEmpty())
    {
        nFlags |=SRV_INFO_FLAG_PWD_REQ;
    }

    emit SendMsg(new CNetMsgSrvInfo(nFlags, ""));
}

void CServer::SendMsgSlot(CNetMsgBase* pMsg)
{
    m_pNet->SendMsg(pMsg);
}

void CServer::OnErrorSlot(QString strTxt)
{
    m_pNet->OnError(strTxt);
}