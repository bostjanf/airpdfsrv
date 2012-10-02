#include "StdAfx.h"
#include "appsettings.h"

CAppSettings* CAppSettings::m_pThis=NULL;

 CAppSettings* CAppSettings::Instance()
 {
     if (m_pThis != NULL)
     {
         return m_pThis;
     }

     m_pThis = new CAppSettings();
     if (!m_pThis->Load())
     {//go to defaults
         m_pThis->CleanHash();
     }
     m_pThis->CheckGuidAtStartup();
     return m_pThis;
 }

 void CAppSettings::CleanUp()
 {
     if (m_pThis != NULL)
     {
         delete m_pThis;
         m_pThis=NULL;
     }
 }


CAppSettings::CAppSettings(void)
    :m_nVersion(1)
{
    m_strFilePath = QCoreApplication::applicationDirPath();
#ifdef AIR_PDF_SRV
    m_strFilePath.append("/srvsettings.data");
#else 
    m_strFilePath.append("/guisettings.data");
#endif
}

CAppSettings::~CAppSettings(void)
{
    CleanHash();
}

QString CAppSettings::GetSrvName()
{
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_NAME;
    if (m_hash.contains(nType))
    {
        CServerNameItem* item = (CServerNameItem*)m_hash[nType];
        if ((item != NULL) && (item->ItemType()==nType))
        {
            return item->GetSrvName();
        }
        Q_ASSERT(NULL);
    }
    return CServerNameItem::DefaultValue();
}

void CAppSettings::SetSrvName(QString strName)
{
    CServerNameItem* item = NULL;
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_NAME;
    if (m_hash.contains(nType))
    {
        item = (CServerNameItem*)m_hash[nType];
    }
    else
    {
         CServerNameItem* item = new CServerNameItem();
         m_hash[nType]=item;
    }
    item->SetSrvName(strName);
    Save();
}

void CAppSettings::SetSrvPort(quint16 nPort)
{
    CServerPortItem* item = NULL;
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_PORT;
    if (m_hash.contains(nType))
    {
        item = (CServerPortItem*)m_hash[nType];
    }
    else
    {
         CServerPortItem* item = new CServerPortItem();
         m_hash[nType]=item;
    }
    item->SetSrvPort(nPort);
    Save();
}

quint16  CAppSettings::GetSrvPort()
{
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_PORT;
    if (m_hash.contains(nType))
    {
        CServerPortItem* item = (CServerPortItem*)m_hash[nType];
        if ((item != NULL) && (item->ItemType()==nType))
        {
            return item->GetSrvPort();
        }
        Q_ASSERT(NULL);
    }
    return CServerPortItem::DefaultValue();
}

bool CAppSettings::AddRootDir(QString strRoot, QString strPath)
{
    CServerRootsDirItem* item = NULL;
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_DIR_ROOTS;
    if (m_hash.contains(nType))
    {
        item = (CServerRootsDirItem*)m_hash[nType];
    }
    else
    {
         item = new CServerRootsDirItem();
         m_hash[nType]=item;
    }
    bool bRet=item->Add(strRoot,strPath);
    Save();

    return bRet;
}

bool CAppSettings::RemoveRootDir(QString strRoot)
{
    CServerRootsDirItem* item = NULL;
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_DIR_ROOTS;
    if (m_hash.contains(nType))
    {
        item = (CServerRootsDirItem*)m_hash[nType];
    }
    else
    {
         return false;
    }
    bool bRet=item->Remove(strRoot);
    Save();
    return bRet;
}

bool CAppSettings::GetRoots( QHash<QString, QString>& hashRoots)
{
    CServerRootsDirItem* item = NULL;
    hashRoots.clear();
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_DIR_ROOTS;
    if (m_hash.contains(nType))
    {
        item = (CServerRootsDirItem*)m_hash[nType];
        return item->GetRoots(hashRoots);
    }
    else
    {
         return false;
    }    
}


void CAppSettings::SetPwd(QByteArray arr)
{
    CPwdItem* item = NULL;
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_PWD;
    if (m_hash.contains(nType))
    {
        item = (CPwdItem*)m_hash[nType];
    }
    else
    {
         CPwdItem* item = new CPwdItem();
         m_hash[nType]=item;
    }
    item->SetPwd(arr);
    Save();
}


QByteArray CAppSettings::GetPwd()
{
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_PWD;
    if (m_hash.contains(nType))
    {
        CPwdItem* item = (CPwdItem*)m_hash[nType];
        if ((item != NULL) && (item->ItemType()==nType))
        {
            return item->GetPwd();
        }
        Q_ASSERT(NULL);
    }
    return QByteArray();
}

bool CAppSettings::IsPwdSet()
{
    return m_hash.contains(SERVER_SETTING_ITEM_TYPE_SRV_PWD);
}

QByteArray CAppSettings::GetGuid()
{
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_GUID;
    if (m_hash.contains(nType))
    {
        CGuidItem* item = (CGuidItem*)m_hash[nType];
        if ((item != NULL) && (item->ItemType()==nType))
        {
            return item->GetGuid();
        }
        Q_ASSERT(NULL);
    }
    else
    {
        CGuidItem* item =  new CGuidItem();
        item->SetGuid(QUuid::createUuid().toByteArray());
        m_hash[nType]=item;
        return item->GetGuid();
    }  
    return QUuid::createUuid().toByteArray();
}

void CAppSettings::CheckGuidAtStartup()
{    
    if (!m_hash.contains(SERVER_SETTING_ITEM_TYPE_SRV_GUID))
    {        
        CGuidItem* item =  new CGuidItem();
        item->SetGuid(QUuid::createUuid().toByteArray());
        m_hash[SERVER_SETTING_ITEM_TYPE_SRV_GUID]=item;
    }
}


void CAppSettings::SetFlags(quint64 nFlag)
{
    CServerFlagsItem* item = NULL;
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_FLAGS;
    if (m_hash.contains(nType))
    {
        item = (CServerFlagsItem*)m_hash[nType];
    }
    else
    {
         CServerFlagsItem* item = new CServerFlagsItem();
         m_hash[nType]=item;
    }
    item->SetFlags(nFlag);
    Save();
}

quint64  CAppSettings::GetFlags()
{
    quint32 nType=SERVER_SETTING_ITEM_TYPE_SRV_FLAGS;
    if (m_hash.contains(nType))
    {
        CServerFlagsItem* item = (CServerFlagsItem*)m_hash[nType];
        if ((item != NULL) && (item->ItemType()==nType))
        {
            return item->GetFlags();
        }
        Q_ASSERT(NULL);
    }
    return CServerFlagsItem::DefaultValue();
}

void CAppSettings::CleanHash()
{
    QHashIterator<quint32, CItem*> it(m_hash);
    while (it.hasNext())
    {
        it.next();
        delete it.value();
    }
    m_hash.clear();
}


bool CAppSettings::Load()
{
    CleanHash();
    QFile file(m_strFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream in(&file);

    quint32 nVersion=0;
    in >> nVersion; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}   

    if (nVersion != m_nVersion)
    {
        return false;
    }

    while (!in.atEnd())
    {
        quint32 nItemType=0;
        in >> nItemType;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}

        if (m_hash.contains(nItemType)){Q_ASSERT(NULL); return false;}        
        CItem* pItem = BuildItem(nItemType);
        if (pItem==NULL) {return false;}
        if (!pItem->Load(in)){return false;}

        m_hash[nItemType]=pItem;
    }
    return true;
}

bool CAppSettings::Save()
{
    QFile file(m_strFilePath);
    if (file.exists())
    {
        QString strRenameTo=m_strFilePath;
        strRenameTo.append("_bak");
        if (QFile::exists(strRenameTo))
        {
            QFile::remove(strRenameTo);
        }
        QFile::rename(m_strFilePath, strRenameTo);
    }

    if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return false;
    }


    QDataStream out(&file);
    out << m_nVersion;
    QHashIterator<quint32, CItem*> it(m_hash);
    while (it.hasNext())
    {
        it.next();
        it.value()->Save(out);
    }

    file.flush();
    file.close();
    return true;
}

CAppSettings::CItem* CAppSettings::BuildItem(quint32 nItemType)
{
    switch (nItemType)
    {
        case SERVER_SETTING_ITEM_TYPE_SRV_NAME:      return new CServerNameItem();
        case SERVER_SETTING_ITEM_TYPE_SRV_PORT:      return new CServerPortItem();
        case SERVER_SETTING_ITEM_TYPE_SRV_DIR_ROOTS: return new CServerRootsDirItem();
        case SERVER_SETTING_ITEM_TYPE_SRV_PWD:       return new CPwdItem();
        case SERVER_SETTING_ITEM_TYPE_SRV_GUID:      return new CGuidItem();
        case SERVER_SETTING_ITEM_TYPE_SRV_FLAGS:     return new CServerFlagsItem();
        default:
        {
            Q_ASSERT(NULL);
            return NULL;
        }
    }
}


/////////SERVER settings items/////////////////////////
CAppSettings::CItem::CItem(SERVER_SETTING_ITEM_TYPE typeIn)
    :m_type(typeIn)
{
}
CAppSettings::CItem::~CItem()
{}

CAppSettings::SERVER_SETTING_ITEM_TYPE CAppSettings::CItem::ItemType()
{
    return m_type;
}

void CAppSettings::CItem::Save(QDataStream& out)
{
    out << (quint32)m_type;  
}

//////////////items server name//////////////////////////////////////
CAppSettings::CServerNameItem::CServerNameItem()
    :CItem(SERVER_SETTING_ITEM_TYPE_SRV_NAME)
{
}

CAppSettings::CServerNameItem::~CServerNameItem()
{}

QString CAppSettings::CServerNameItem::DefaultValue()
{
    return  QHostInfo::localHostName(); 
}

void CAppSettings::CServerNameItem::SetSrvName(QString strSrvName)
{
    m_strSrvName=strSrvName;
}

QString CAppSettings::CServerNameItem::GetSrvName()
{
    return m_strSrvName;
}

void CAppSettings::CServerNameItem::Save(QDataStream& out)
{
    CItem::Save(out);
    out << m_strSrvName;  
}

bool CAppSettings::CServerNameItem::Load(QDataStream& in)
{
    in >> m_strSrvName; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    return true;
}
//////////////items server port//////////////////////////////////////
CAppSettings::CServerPortItem::CServerPortItem()
    :CItem(SERVER_SETTING_ITEM_TYPE_SRV_PORT)
{
}

CAppSettings::CServerPortItem::~CServerPortItem()
{}

quint16 CAppSettings::CServerPortItem::DefaultValue()
{
    return AIRPDF_COMMUNICATE_PORT; 
}

void CAppSettings::CServerPortItem::SetSrvPort(quint16 nPort)
{
    m_nPort=nPort;
}

quint16 CAppSettings::CServerPortItem::GetSrvPort()
{
    return m_nPort;
}

void CAppSettings::CServerPortItem::Save(QDataStream& out)
{
    CItem::Save(out);
    out << m_nPort;  
}

bool CAppSettings::CServerPortItem::Load(QDataStream& in)
{
    in >> m_nPort; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    return true;
}

//////////////items server dir roots//////////////////////////////////////
CAppSettings::CServerRootsDirItem::CServerRootsDirItem()
    :CItem(SERVER_SETTING_ITEM_TYPE_SRV_DIR_ROOTS)
{
}

CAppSettings::CServerRootsDirItem::~CServerRootsDirItem()
{}

bool CAppSettings::CServerRootsDirItem::Add(QString strRoot, QString strPath)
{
    if (!m_hashRoots.contains(strRoot))
    {
        m_hashRoots[strRoot]=strPath;        
        return true;
    }
    return false;
}

bool CAppSettings::CServerRootsDirItem::Remove(QString strRoot)
{
    if (m_hashRoots.contains(strRoot))
    {
        m_hashRoots.remove(strRoot);
        return true;
    }
    return false;
}

bool CAppSettings::CServerRootsDirItem::GetRoots( QHash<QString, QString>& hashRoots)
{
    hashRoots.clear();
    QHashIterator<QString, QString> it(m_hashRoots);
    while (it.hasNext())
    {
        it.next();
        hashRoots[it.key()]=it.value();
    }
    return !hashRoots.isEmpty();
}

void CAppSettings::CServerRootsDirItem::Save(QDataStream& out)
{
    CItem::Save(out);
    quint32 nSize =  m_hashRoots.size();
    out << nSize;  
    QHashIterator<QString, QString> it(m_hashRoots);
    while (it.hasNext())
    {
        it.next();
        out << it.key();
        out << it.value();
    }   
}

bool CAppSettings::CServerRootsDirItem::Load(QDataStream& in)
{
    quint32 nSize =0;
    in >> nSize; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}   
    m_hashRoots.clear();
    for (quint32 idx=0; idx<nSize; idx++)
    {
        QString strKey, strVal;
        in >> strKey; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}  
        in >> strVal; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}  
        m_hashRoots[strKey]=strVal;
    }
    return true;
}


//////////////item pwd//////////////////////////////////////
CAppSettings::CPwdItem::CPwdItem()
    :CItem(SERVER_SETTING_ITEM_TYPE_SRV_PWD)
{
}

CAppSettings::CPwdItem::~CPwdItem()
{}

void CAppSettings::CPwdItem::SetPwd(QByteArray arr)
{
    m_arrPwd=arr;
}

QByteArray CAppSettings::CPwdItem::GetPwd()
{
    return m_arrPwd;
}

void CAppSettings::CPwdItem::Save(QDataStream& out)
{
    CItem::Save(out);
    out << m_arrPwd;  
}

bool CAppSettings::CPwdItem::Load(QDataStream& in)
{
    in >> m_arrPwd; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    return true;
}


//////////////item guid//////////////////////////////////////
CAppSettings::CGuidItem::CGuidItem()
    :CItem(SERVER_SETTING_ITEM_TYPE_SRV_GUID)
{
}

CAppSettings::CGuidItem::~CGuidItem()
{}

void CAppSettings::CGuidItem::SetGuid(QByteArray arr)
{
    m_arrGuid=arr;
}

QByteArray CAppSettings::CGuidItem::GetGuid()
{
    return m_arrGuid;
}

void CAppSettings::CGuidItem::Save(QDataStream& out)
{
    CItem::Save(out);
    out << m_arrGuid;  
}

bool CAppSettings::CGuidItem::Load(QDataStream& in)
{
    in >> m_arrGuid; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    return true;
}


//////////////items server flags//////////////////////////////////////
CAppSettings::CServerFlagsItem::CServerFlagsItem()
    :CItem(SERVER_SETTING_ITEM_TYPE_SRV_PORT)
{
}

CAppSettings::CServerFlagsItem::~CServerFlagsItem()
{}

quint64 CAppSettings::CServerFlagsItem::DefaultValue()
{
    return -1; 
}

void CAppSettings::CServerFlagsItem::SetFlags(quint64 nFlags)
{
    m_nFlags=nFlags;
}

quint64 CAppSettings::CServerFlagsItem::GetFlags()
{
    return m_nFlags;
}

void CAppSettings::CServerFlagsItem::Save(QDataStream& out)
{
    CItem::Save(out);
    out << m_nFlags;  
}

bool CAppSettings::CServerFlagsItem::Load(QDataStream& in)
{
    in >> m_nFlags; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    return true;
}
