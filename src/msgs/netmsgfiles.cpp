#include "stdafx.h"
#include "netmsgfiles.h"

///////////////////////CNetMsgReqDir////////////////////////////
CNetMsgReqDir::CNetMsgReqDir(QString strRoot, QString strRelPath)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_REQ_DIR),m_strRoot(strRoot),m_strRelPath(strRelPath)
{

}

CNetMsgReqDir::CNetMsgReqDir(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{
    
}

CNetMsgReqDir::~CNetMsgReqDir(void)
{
}

QString CNetMsgReqDir::RelativeDir()
{
    return m_strRelPath;
}

QString CNetMsgReqDir::Root()
{
    return m_strRoot;
}

QString CNetMsgReqDir::Path()
{
    QString strRet(m_strRoot);
    if (!m_strRelPath.isEmpty())
    {
        strRet.append("/");
        strRet.append(m_strRelPath);
    }
    return strRet;
}

bool CNetMsgReqDir::LoadMsg(QDataStream& in)
{
    if (!ReadString(m_strRoot,in))   {Q_ASSERT(NULL);return false;}
    if (!ReadString(m_strRelPath,in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgReqDir::SaveMsg(QDataStream& out)
{
    if (!WriteString(m_strRoot,out))   {Q_ASSERT(NULL);return false;}
    if (!WriteString(m_strRelPath,out)){Q_ASSERT(NULL);return false;}
    return true;
}

///////////////////////CNetMsgDirFiles::CItem////////////////////////////
CNetMsgDirFiles::CItem::CItem()
    :nType(0), nSize(0)
{
}

CNetMsgDirFiles::CItem::CItem(QString strNameIn, quint8 nTypeIn, quint32 nSize)
    :strName(strNameIn),nType(nTypeIn), nSize(nSize)
{
}

bool CNetMsgDirFiles::CItem::LoadMsg(QDataStream& in)
{
     in >> nType; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
     in >> nSize; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
     if (!CNetMsgBaseBuffered::ReadString(strName,in))   {Q_ASSERT(NULL);return false;}    

    return true;
}

bool CNetMsgDirFiles::CItem::SaveMsg(QDataStream& out)
{
    out << nType;
    out << nSize; 
    if (!CNetMsgBaseBuffered::WriteString(strName,out))   {Q_ASSERT(NULL);return false;} 
    return true;
}

CNetMsgDirFiles::CNetMsgDirFiles(QList<CNetMsgDirFiles::CItem> list, bool bIsRootDir)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_DIR_FILES), m_listItems(list)
{
    m_bIsRootDir = bIsRootDir?1:0;
}

CNetMsgDirFiles::CNetMsgDirFiles(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{
    
}

CNetMsgDirFiles::~CNetMsgDirFiles(void)
{
}
bool CNetMsgDirFiles::Items(ConstItemList& list)
{
    list.clear();
    for (quint32 idx=0; idx<m_listItems.size(); idx++)
    {
        list.append(&(m_listItems.at(idx)));
    }
    return !list.isEmpty();
}

bool CNetMsgDirFiles::IsRootDir()
{
    return m_bIsRootDir!=0;
}



bool CNetMsgDirFiles::LoadMsg(QDataStream& in)
{
    in >> m_bIsRootDir;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    quint32 nItems =0;
    in >> nItems;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    m_listItems.clear();
    for (quint32 idx=0; idx<nItems; idx++)
    {
        CItem item;
        if (!item.LoadMsg(in)){Q_ASSERT(NULL);return false;}
        m_listItems.append(item);
    }
    return true;
}

bool CNetMsgDirFiles::SaveMsg(QDataStream& out)
{    
    out << m_bIsRootDir;
    quint32 nItems = m_listItems.size();
    out << nItems;
    for (quint32 idx=0; idx<nItems; idx++)
    {
        if (!m_listItems[idx].SaveMsg(out)){Q_ASSERT(NULL);return false;}
    }
    return true;
}