#include "stdafx.h"
#include "netmsgversions.h"
#include "appsettings.h"

///////////////////////CNetMsgSrvVersion////////////////////////////
CNetMsgSrvVersion::CNetMsgSrvVersion()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_SRV_VERSION),m_nVersion(0),m_nOS(0),m_nFlags(0)
{
    m_nVersion = PROTOCOL_VERSION;

#if defined(Q_WS_WIN)
    m_nOS = PROTOCOL_MACHINE_OS_WINDOWS;
#elif defined(Q_WS_MAC)
     m_nOS = PROTOCOL_MACHINE_OS_OSX;
#elif if defined(Q_WS_X11)
     m_nOS = PROTOCOL_MACHINE_OS_LINUX;
#endif;

#ifdef AIR_PDF_SRV     
    m_strGuid = QString(CAppSettings::Instance()->GetGuid().toHex()); 
    m_nFlags= CAppSettings::Instance()->GetFlags(); 
#endif

}

CNetMsgSrvVersion::CNetMsgSrvVersion(CNetHeader& header)
    :CNetMsgBaseBuffered(header),m_nVersion(0),m_nOS(0)
{
    
}

CNetMsgSrvVersion::~CNetMsgSrvVersion(void)
{
}

quint16 CNetMsgSrvVersion::MajorVersion()
{
    return m_nVersion/255;
}

quint16 CNetMsgSrvVersion::MinorVersion()
{
    return (m_nVersion%255);
}

bool CNetMsgSrvVersion::LoadMsg(QDataStream& in)
{
    in >> m_nVersion;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    in >> m_nOS;     if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    in >> m_nFlags;  if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    if (!ReadString(m_strGuid,in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgSrvVersion::SaveMsg(QDataStream& out)
{
    out << m_nVersion;
    out << m_nOS;   
    out << m_nFlags;   
    if (!WriteString(m_strGuid,out)){Q_ASSERT(NULL);return false;}
    return true;
}


///////////////////////CNetMsgGuiVersion////////////////////////////
CNetMsgGuiVersion::CNetMsgGuiVersion()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_GUI_VERSION),m_nVersion(0)
{
     m_nVersion = 1*255 +1;
}

CNetMsgGuiVersion::CNetMsgGuiVersion(CNetHeader& header)
    :CNetMsgBaseBuffered(header),m_nVersion(0)
{
}

CNetMsgGuiVersion::~CNetMsgGuiVersion(void)
{
}

quint16 CNetMsgGuiVersion::MajorVersion()
{
    return m_nVersion/255;
}

quint16 CNetMsgGuiVersion::MinorVersion()
{
    return m_nVersion%255;
}

bool CNetMsgGuiVersion::LoadMsg(QDataStream& in)
{
    in >> m_nVersion;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    return true;
}

bool CNetMsgGuiVersion::SaveMsg(QDataStream& out)
{
    out << m_nVersion;
    return true;
}

///////////////////////CNetMsgIosVersion////////////////////////////
CNetMsgIosVersion::CNetMsgIosVersion()
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_IOS_VERSION),m_nVersion(0),m_nDev(0)
{
}

CNetMsgIosVersion::CNetMsgIosVersion(CNetHeader& header)
    :CNetMsgBaseBuffered(header),m_nVersion(0),m_nDev(0)
{
}

CNetMsgIosVersion::~CNetMsgIosVersion(void)
{
}

quint16 CNetMsgIosVersion::MajorVersion()
{
    return m_nVersion/255;
}

quint16 CNetMsgIosVersion::MinorVersion()
{
    return m_nVersion%255;
}

bool CNetMsgIosVersion::LoadMsg(QDataStream& in)
{
    in >> m_nVersion;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    in >> m_nDev;    if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    return true;
}

bool CNetMsgIosVersion::SaveMsg(QDataStream& out)
{
    out << m_nVersion;
    out << m_nDev; 
    return true;
}


///////////////////////CNetMsgError////////////////////////////
CNetMsgError::CNetMsgError(QString strError)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_ERROR),m_strError(strError)
{
}

CNetMsgError::CNetMsgError(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{    
}
CNetMsgError::~CNetMsgError(void)
{
}


bool CNetMsgError::LoadMsg(QDataStream& in)
{
    if (!ReadString(m_strError,in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgError::SaveMsg(QDataStream& out)
{ 
    if (!WriteString(m_strError,out)){Q_ASSERT(NULL);return false;}
    return true;
}