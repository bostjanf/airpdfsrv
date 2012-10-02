#include "stdafx.h"
#include "netmsgsettings.h"


///////////////////////CNetMsgSettings////////////////////////////
CNetMsgSettings::CNetMsgSettings()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_SETTINGS),m_nSet(0),m_nFlags(0)
{

}

CNetMsgSettings::CNetMsgSettings(quint8 nSet, quint64 nFlags, QString strSrvName)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_SETTINGS),
    m_nSet(nSet),m_nFlags(nFlags),m_strSrvName(strSrvName)
{

}

CNetMsgSettings::CNetMsgSettings(CNetHeader& header)
    :CNetMsgBaseBuffered(header),m_nSet(0),m_nFlags(0)
{
    
}

CNetMsgSettings::~CNetMsgSettings(void)
{
}


bool CNetMsgSettings::LoadMsg(QDataStream& in)
{
    in >> m_nSet;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    in >> m_nFlags;     if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    if (!ReadString(m_strSrvName,in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgSettings::SaveMsg(QDataStream& out)
{
    out << m_nSet;
    out << m_nFlags;   
    if (!WriteString(m_strSrvName,out)){Q_ASSERT(NULL);return false;}
    return true;
}


///////////////////////CNetMsgAddDelRootDir////////////////////////////
CNetMsgAddDelRootDir::CNetMsgAddDelRootDir()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_ADD_DEL_ROOT_DIR),
    m_nStatus(1)
{
}

CNetMsgAddDelRootDir::CNetMsgAddDelRootDir(quint8  nStatus, QString strRootName, QString strRootPath)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_ADD_DEL_ROOT_DIR),
     m_nStatus(nStatus), m_strRootName(strRootName),m_strRootPath(strRootPath)
{   
}

CNetMsgAddDelRootDir::CNetMsgAddDelRootDir(CNetHeader& header)
    :CNetMsgBaseBuffered(header),m_nStatus(0)
{
}

CNetMsgAddDelRootDir::~CNetMsgAddDelRootDir(void)
{
}

bool CNetMsgAddDelRootDir::LoadMsg(QDataStream& in)
{
    in >> m_nStatus;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    if (m_nStatus)
    {
        if (!ReadString(m_strRootName,in)){Q_ASSERT(NULL);return false;}
        if (!ReadString(m_strRootPath,in)){Q_ASSERT(NULL);return false;}
    }
    return true;
}

bool CNetMsgAddDelRootDir::SaveMsg(QDataStream& out)
{   
    out << m_nStatus;   
    if (m_nStatus)
    {
        if (!WriteString(m_strRootName,out)){Q_ASSERT(NULL);return false;}
        if (!WriteString(m_strRootPath,out)){Q_ASSERT(NULL);return false;}
    }
    return true;
}


///////////////////////CNetMsgRootDirList////////////////////////////
CNetMsgRootDirList::CItem::CItem()
{}
CNetMsgRootDirList::CItem::CItem(QString strRoot, QString strPath)
    :m_strRoot(strRoot),m_strPath(strPath)
{}
CNetMsgRootDirList::CItem::~CItem()
{}

bool CNetMsgRootDirList::CItem::Load(QDataStream& in)
{
    if (!ReadString(m_strRoot,in)){Q_ASSERT(NULL);return false;}
    if (!ReadString(m_strPath,in)){Q_ASSERT(NULL);return false;}
    return true;
}
bool CNetMsgRootDirList::CItem::Save(QDataStream& out)
{
    if (!WriteString(m_strRoot,out)){Q_ASSERT(NULL);return false;}
    if (!WriteString(m_strPath,out)){Q_ASSERT(NULL);return false;}
    return true;
}

CNetMsgRootDirList::CNetMsgRootDirList()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_ROOT_DIR_LIST)
{
}

CNetMsgRootDirList::CNetMsgRootDirList(QList<CNetMsgRootDirList::CItem> list)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_ROOT_DIR_LIST)
{    
    m_list.append(list);
}

CNetMsgRootDirList::CNetMsgRootDirList(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{
    
}

CNetMsgRootDirList::~CNetMsgRootDirList(void)
{
}

bool CNetMsgRootDirList::GetList(QList<CNetMsgRootDirList::CItem>& list)
{
    list.clear();
    list.append(m_list);
    return !list.isEmpty();
}

bool CNetMsgRootDirList::LoadMsg(QDataStream& in)
{    
    m_list.clear();
    quint32 nSize = 0;
    in >> nSize;
    for (quint32 idx=0; idx<nSize; idx++)
    {
        CItem item;
        if (!item.Load(in)){Q_ASSERT(NULL);return false;}
        m_list.append(item);
    }    
    return true;
}

bool CNetMsgRootDirList::SaveMsg(QDataStream& out)
{   
    quint32 nSize = m_list.size();
    out << nSize;
    for (quint32 idx=0; idx<nSize; idx++)
    {      
        CItem item  =m_list.at(idx);
        item.Save(out);
    }        
    return true;
}


///////////////////////CNetMsgReqMountPoints////////////////////////////
CNetMsgReqMountPoints::CNetMsgReqMountPoints()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_REQ_MOUNT_POINTS)
{

}

CNetMsgReqMountPoints::CNetMsgReqMountPoints(QString strPath)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_REQ_MOUNT_POINTS),
      m_strPath(strPath)
{    
}

CNetMsgReqMountPoints::CNetMsgReqMountPoints(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{
    
}

CNetMsgReqMountPoints::~CNetMsgReqMountPoints(void)
{
}

bool CNetMsgReqMountPoints::LoadMsg(QDataStream& in)
{    
    if (!ReadString(m_strPath,in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgReqMountPoints::SaveMsg(QDataStream& out)
{   
    if (!WriteString(m_strPath,out)){Q_ASSERT(NULL);return false;}    
    return true;
}

///////////////////////CNetMsgMountPoints////////////////////////////
CNetMsgMountPoints::CNetMsgMountPoints()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_MOUNT_POINTS)
{

}

CNetMsgMountPoints::CNetMsgMountPoints(QString strPath, QStringList listPaths)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SETT_MOUNT_POINTS),
      m_strPath(strPath), m_listPaths(listPaths)
{    
}

CNetMsgMountPoints::CNetMsgMountPoints(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{
    
}

CNetMsgMountPoints::~CNetMsgMountPoints(void)
{
}

bool CNetMsgMountPoints::LoadMsg(QDataStream& in)
{    
    if (!ReadString(m_strPath,in)){Q_ASSERT(NULL);return false;}
    quint32 nItems=0;
    m_listPaths.clear();
    in >> nItems;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    for (quint32 idx=0; idx<nItems; idx++)
    {
        QString strItem;
        if (!ReadString(strItem,in)){Q_ASSERT(NULL);return false;}
        m_listPaths.append(strItem);
    }

    return true;
}

bool CNetMsgMountPoints::SaveMsg(QDataStream& out)
{   
    if (!WriteString(m_strPath,out)){Q_ASSERT(NULL);return false;}
    quint32 nItems=m_listPaths.size();
    out << nItems;
    for (quint32 idx=0; idx<nItems; idx++)
    {
         QString strItem = m_listPaths.at(idx);
        if (!WriteString(strItem,out)){Q_ASSERT(NULL);return false;}     
    }
    return true;
}
