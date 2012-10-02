#include "stdafx.h"
#include "netmsgbooks.h"

///////////////////////CNetMsgReqBookInfo////////////////////////////
CNetMsgReqBookInfo::CNetMsgReqBookInfo(QString strRoot, QString strPath)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_REQ_BOOK_INFO),m_strRoot(strRoot),m_strPath(strPath)
{
}

CNetMsgReqBookInfo::CNetMsgReqBookInfo(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{    
}
CNetMsgReqBookInfo::~CNetMsgReqBookInfo(void)
{
}
QString CNetMsgReqBookInfo::Root(){return m_strRoot;}
QString CNetMsgReqBookInfo::Path(){return m_strPath;}

bool CNetMsgReqBookInfo::LoadMsg(QDataStream& in)
{
    if (!ReadString(m_strRoot,in))   {Q_ASSERT(NULL);return false;}
    if (!ReadString(m_strPath,in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgReqBookInfo::SaveMsg(QDataStream& out)
{
    if (!WriteString(m_strRoot,out)){Q_ASSERT(NULL);return false;}
    if (!WriteString(m_strPath,out)){Q_ASSERT(NULL);return false;}
    return true;
}

///////////////////////CNetMsgBookInfo////////////////////////////
CNetMsgBookInfo::CNetMsgBookInfo(quint32 nPages)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_BOOK_INFO),m_nPages(nPages)
{
}

CNetMsgBookInfo::CNetMsgBookInfo(QString strError)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_BOOK_INFO),m_strError(strError),m_nPages(0)
{
}

CNetMsgBookInfo::CNetMsgBookInfo(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{    
}
CNetMsgBookInfo::~CNetMsgBookInfo(void)
{
}

bool CNetMsgBookInfo::LoadMsg(QDataStream& in)
{
    in >> m_nPages; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}    
    if (!ReadString(m_strError, in)){Q_ASSERT(NULL);return false;}
    return true;
}

bool CNetMsgBookInfo::SaveMsg(QDataStream& out)
{
    out << m_nPages;
    if (!WriteString(m_strError, out)){Q_ASSERT(NULL);return false;}
    return true;
}


///////////////////////CNetMsgReqBookPage////////////////////////////
CNetMsgReqBookPage::CNetMsgReqBookPage(QString strRoot, QString strPath, quint32 nPage, quint16 nDpi, quint8 nFlags)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_REQ_BOOK_PAGE),m_strRoot(strRoot),m_strPath(strPath),m_nPage(nPage),m_nDpi(nDpi),m_nFlags(nFlags)
{
}

CNetMsgReqBookPage::CNetMsgReqBookPage(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{    
}
CNetMsgReqBookPage::~CNetMsgReqBookPage(void)
{
}


QString CNetMsgReqBookPage::Root() {return m_strRoot;}
QString CNetMsgReqBookPage::Path() {return m_strPath;}
quint32 CNetMsgReqBookPage::Page(){return m_nPage;} 
quint16 CNetMsgReqBookPage::Dpi(){return m_nDpi;} 
quint8 CNetMsgReqBookPage::Flags(){return m_nFlags;} 

bool CNetMsgReqBookPage::LoadMsg(QDataStream& in)
{
    if (!ReadString(m_strRoot,in)){Q_ASSERT(NULL);return false;}
    if (!ReadString(m_strPath,in)){Q_ASSERT(NULL);return false;}

     in >> m_nPage; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
     in >> m_nDpi; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
     in >> m_nFlags; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    return true;
}

bool CNetMsgReqBookPage::SaveMsg(QDataStream& out)
{
    if (!WriteString(m_strRoot, out)){Q_ASSERT(NULL);return false;}
    if (!WriteString(m_strPath,out)){Q_ASSERT(NULL);return false;}
    out << m_nPage;
    out << m_nDpi;
    out << m_nFlags;
    return true;
}

///////////////////////CNetMsgBookPage////////////////////////////
const quint32 CNetMsgBookPage::SEND_BUFF_SIZE=64*1024;
CNetMsgBookPage::CNetMsgBookPage(QFile* pFile)
    :CNetMsgBase(AIRPDF_MSG_ID_BOOK_PAGE),m_pFileToSend(pFile),m_nDataMissing(0)
{
    m_header.m_nMsgLen |=MSG_FLAG_STREAM_MSG;
}

CNetMsgBookPage::CNetMsgBookPage(CNetHeader& header)
    :CNetMsgBase(header),m_pFileToSend(NULL)
{   
    m_nDataMissing = header.m_nMsgLen-CNetHeader::HEADER_SIZE;
}

CNetMsgBookPage::~CNetMsgBookPage(void)
{
    if (m_pFileToSend)
    {
        m_pFileToSend->close();
        m_pFileToSend->remove();
        delete m_pFileToSend;
        m_pFileToSend=NULL;
    }
    m_arr.clear();
}

qint8 CNetMsgBookPage::OnReceivedData(QBuffer& buffer, quint32 nSize)
{        
    if (m_nDataMissing<nSize)
    {
        nSize = m_nDataMissing;
    }

    m_arr.append(buffer.read(nSize));
    m_nDataMissing -=nSize;

    if (m_nDataMissing)
    {
        return 1;//not all data yet
    }
    return 0;
}

bool  CNetMsgBookPage::DataForSend(QByteArray** pData)
{
    if (m_pFileToSend==NULL)
    {
        return false;
    }
    m_arr.clear();
    m_arr.reserve(SEND_BUFF_SIZE);
    if (m_pFileToSend->pos())
    {
        m_arr.append(m_pFileToSend->read(SEND_BUFF_SIZE));
    }
    else
    {//send header first and then append file buff
        quint32 nReadOut=SEND_BUFF_SIZE-CNetHeader::HEADER_SIZE;
        m_header.m_nMsgLen=m_pFileToSend->size()+CNetHeader::HEADER_SIZE;
        m_header.Save(m_arr);
        m_arr.append(m_pFileToSend->read(nReadOut));
    }    
    *pData = &m_arr;
    return true;
}

bool  CNetMsgBookPage::OnBytesSend(qint64 nSend)
{
    if (m_pFileToSend==NULL)
    {
        Q_ASSERT(NULL);
        return false;
    }    
    return m_pFileToSend->atEnd();
}

///////////////////////CNetMsgBookCalcDpi////////////////////////////
CNetMsgBookCalcDpi::CNetMsgBookCalcDpi(QString strRoot, QString strPath, quint32 nPage, quint16 nLargestSize)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_BOOK_CALC_DPI),m_strRoot(strRoot),m_strPath(strPath),
    m_nPage(nPage), m_nLargestSize(nLargestSize)
{
}

CNetMsgBookCalcDpi::CNetMsgBookCalcDpi(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{    
}
CNetMsgBookCalcDpi::~CNetMsgBookCalcDpi(void)
{
}

QString CNetMsgBookCalcDpi::Root() {return m_strRoot;}
QString CNetMsgBookCalcDpi::Path() {return m_strPath;}
quint32 CNetMsgBookCalcDpi::Page(){return m_nPage;} 
quint16 CNetMsgBookCalcDpi::LargestSize(){return m_nLargestSize;} 

bool CNetMsgBookCalcDpi::LoadMsg(QDataStream& in)
{
    if (!ReadString(m_strRoot,in)){Q_ASSERT(NULL);return false;}
    if (!ReadString(m_strPath,in)){Q_ASSERT(NULL);return false;}

    in >> m_nPage; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    in >> m_nLargestSize; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    return true;
}

bool CNetMsgBookCalcDpi::SaveMsg(QDataStream& out)
{ 
    if (!WriteString(m_strRoot, out)){Q_ASSERT(NULL);return false;}
    if (!WriteString(m_strPath,out)){Q_ASSERT(NULL);return false;}
    out << m_nPage;
    out << m_nLargestSize;

    return true;
}

///////////////////////CNetMsgBookDpi////////////////////////////
CNetMsgBookDpi::CNetMsgBookDpi(quint32 nPage, quint16 nDpi)
    :CNetMsgBaseBuffered(AIRPDF_MSG_ID_BOOK_DPI),m_nPage(nPage),m_nDpi(nDpi)
{
}

CNetMsgBookDpi::CNetMsgBookDpi(CNetHeader& header)
    :CNetMsgBaseBuffered(header)
{    
}
CNetMsgBookDpi::~CNetMsgBookDpi(void)
{
}

quint32 CNetMsgBookDpi::Page(){return m_nPage;} 
quint16 CNetMsgBookDpi::Dpi(){return m_nDpi;} 


bool CNetMsgBookDpi::LoadMsg(QDataStream& in)
{
    in >> m_nPage; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    in >> m_nDpi; if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    return true;
}

bool CNetMsgBookDpi::SaveMsg(QDataStream& out)
{ 
    out << m_nPage;
    out << m_nDpi;

    return true;
}