#include "stdafx.h"
#include "netmsgbase.h"

#include "netmsgdefines.h"
#include "netmsgversions.h"
#include "netmsgfiles.h"
#include "netmsgbooks.h"
#include "netmsgsettings.h"

///////////////////////CNetHeader////////////////////////////
const quint32 CNetHeader::HEADER_SIZE=sizeof(quint32)+sizeof(quint8)+sizeof(quint8);

CNetHeader::CNetHeader(void)
:m_nMsgId(0),m_nFlags(0),m_nMsgLen(0)
{
}

CNetHeader::CNetHeader(quint8 nMsgId)
:m_nMsgId(nMsgId),m_nFlags(0)
{
}


CNetHeader::~CNetHeader(void)
{
}


bool CNetHeader::Load(QBuffer& buffer)
{
    quint32 nRead = buffer.read((char*)&m_nMsgLen, sizeof(m_nMsgLen));
    //if (buffer.read((char*)&m_nMsgLen, sizeof(m_nMsgLen)) != sizeof(m_nMsgLen)){return false;}
    if (buffer.read((char*)&m_nMsgId, sizeof(m_nMsgId)) != sizeof(m_nMsgId)){return false;}
    if (buffer.read((char*)&m_nFlags, sizeof(m_nFlags)) != sizeof(m_nFlags)){return false;}

    if (m_nMsgLen < HEADER_SIZE)
    {
        Q_ASSERT(NULL);
        return false;
    }

    if ((m_nMsgId < AIRPDF_MSG_ID_SRV_VERSION) ||  
        (m_nMsgId>AIRPDF_MSG_LAST_MSG))
    {
        Q_ASSERT(NULL);
        return false;
    }
    
    if ((m_nFlags&MSG_FLAG_STREAM_MSG)==0)
    {//none stream msg has  10485760 =10MB limit
        if (m_nMsgLen>10485760)
        {
            Q_ASSERT(NULL);
            return false;
        }
    }
    return true;
}

bool CNetHeader::Save(QByteArray& arr)
{    
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    return Save(out);
}

bool CNetHeader::Save(QDataStream& out)
{
    out << m_nMsgLen;
    out << m_nMsgId;
    out << m_nFlags;

    return  true;
}



///////////////////////CNetMsgBase////////////////////////////
CNetMsgBase::CNetMsgBase(quint8 m_nMsgId)
    :m_header(m_nMsgId)
{
}

CNetMsgBase::CNetMsgBase(CNetHeader& header)
    :m_header(header)
{
}

CNetMsgBase::~CNetMsgBase(void)
{
}

quint16 CNetMsgBase::MsgId()
{
    return m_header.m_nMsgId;
}
///////////////////////CNetMsgBaseBuffered////////////////////////////
CNetMsgBaseBuffered::CNetMsgBaseBuffered(quint8 nMsgId)
    :CNetMsgBase(nMsgId),m_pArrData(NULL)
{
}
CNetMsgBaseBuffered::CNetMsgBaseBuffered(CNetHeader& header)
    :CNetMsgBase(header),m_pArrData(NULL)
{
}
CNetMsgBaseBuffered::~CNetMsgBaseBuffered(void)
{
    if (m_pArrData != NULL)
    {
        delete m_pArrData;
        m_pArrData=NULL;
    }
}

bool CNetMsgBaseBuffered::WriteString(QString& str, QDataStream& out)
{
    if (str.isEmpty())
    {
         out << (quint16)0;
        return true;
    }
    QByteArray utf8 = str.toUtf8();
    utf8.append((char)0);//null terminated string
    quint16 nSize = utf8.size();
    out << nSize;
    out.writeRawData(utf8.constData(), utf8.size());
    return true;
}

bool CNetMsgBaseBuffered::ReadString(QString& str,QDataStream& in)
{
    quint16 nSize = str.size();
    in >> nSize;if(in.status()!=QDataStream::Ok){Q_ASSERT(NULL); return false;}
    if (nSize)
    {
        QByteArray data;
        data.resize(nSize);
        if (in.readRawData(data.data(), nSize)==nSize)
        {
            str = QString::fromUtf8 (data.constData(),(nSize-1));//null terminated string
            return true;
        }
        Q_ASSERT(NULL);
        return false;
    }
    else
    {
        str.clear();
    }
    return true;
}

qint8 CNetMsgBaseBuffered::OnReceivedData(QBuffer& buffer, quint32 nSize)
{
    qint32 nMsgLen = m_header.m_nMsgLen-CNetHeader::HEADER_SIZE;
    if ((nMsgLen > 10485760) || (nMsgLen < 1)) //10 MB limit for buffered msgs
    {
        Q_ASSERT(NULL);
        AIRPDF_LOG(LOG_LEVEL_ERROR,  QString("Invalid msg %1 . Msg len %2 excited.").arg(m_header.m_nMsgId).arg(m_header.m_nMsgLen));
        return -1;
    }
    if (m_pArrData== NULL)
    {
        m_pArrData = new QByteArray();
        m_pArrData->reserve(nMsgLen);
    }

    qint32 nRead = nMsgLen-m_pArrData->size();
    if (nSize < nRead)
    {
        nRead = nSize;
    }
    m_pArrData->append(buffer.read(nRead));    
    if (m_pArrData->size() == nMsgLen)
    {
        qint8 nRet = -1;
        {
            QDataStream inStream(m_pArrData, QIODevice::ReadOnly);
            inStream.setByteOrder(QDataStream::LittleEndian);
            if (LoadMsg(inStream))
            {
                nRet = 0;
            }
            else
            {
                AIRPDF_LOG(LOG_LEVEL_ERROR,  QString("Invalid msg %1 . Msg format error.").arg(m_header.m_nMsgId));
            }
        }
        delete m_pArrData;
        m_pArrData = NULL;
        return nRet;
    }
    return 1;//not all data yet
}

bool  CNetMsgBaseBuffered::DataForSend(QByteArray** pData)
{
    if (m_pArrData != NULL)
    {
        Q_ASSERT(NULL);
        delete m_pArrData;
    }
    m_pArrData = new QByteArray();
    *pData = m_pArrData;
    QDataStream outStream(m_pArrData, QIODevice::WriteOnly);
    outStream.setByteOrder(QDataStream::LittleEndian);
    if (m_header.Save(outStream) && SaveMsg(outStream))
    {
        quint32 nMsgLen = m_pArrData->size();
        memcpy(m_pArrData->data(),(char*)&nMsgLen, sizeof(nMsgLen));
        return true;
    }
    Q_ASSERT(NULL);
    return false;
}

bool  CNetMsgBaseBuffered::OnBytesSend(qint64 nSend)
{
    if (m_pArrData != NULL)
    {
        if (m_pArrData->size() == nSend)
        {
            delete m_pArrData;
            m_pArrData=NULL;
            return true;
        }
    }
    Q_ASSERT(NULL);
    return false;
}

///////////////////////CNetMsgFactory////////////////////////////
CNetMsgBase* CNetMsgFactory::CreateMsg(QBuffer& buffer)
{
    CNetHeader header;
    if (!header.Load(buffer))
    {
        Q_ASSERT(NULL);
        return false;
    }

    switch (header.m_nMsgId)
    {
        case AIRPDF_MSG_ID_SRV_VERSION: return new CNetMsgSrvVersion(header);
        case AIRPDF_MSG_ID_GUI_VERSION: return new CNetMsgGuiVersion(header);
        case AIRPDF_MSG_ID_IOS_VERSION: return new CNetMsgIosVersion(header);
        case AIRPDF_MSG_ID_SRV_INFO: return new CNetMsgSrvInfo(header);
        case AIRPDF_MSG_ID_ERROR: return new CNetMsgError(header);

        case AIRPDF_MSG_ID_REQ_DIR: return new CNetMsgReqDir(header);
        case AIRPDF_MSG_ID_DIR_FILES: return new CNetMsgDirFiles(header);

        case AIRPDF_MSG_ID_REQ_BOOK_INFO: return new CNetMsgReqBookInfo(header);
        case AIRPDF_MSG_ID_BOOK_INFO: return new CNetMsgBookInfo(header);
        case AIRPDF_MSG_ID_BOOK_PASS: return NULL;
        case AIRPDF_MSG_ID_REQ_BOOK_PAGE: return new CNetMsgReqBookPage(header);        
        case AIRPDF_MSG_ID_BOOK_PAGE: return new CNetMsgBookPage(header);
        case AIRPDF_MSG_ID_BOOK_CALC_DPI: return new CNetMsgBookCalcDpi(header);
        case AIRPDF_MSG_ID_BOOK_DPI: return new CNetMsgBookDpi(header);

        case AIRPDF_MSG_ID_SETT_SETTINGS:return new CNetMsgSettings(header);
        case AIRPDF_MSG_ID_SETT_ADD_DEL_ROOT_DIR:return new CNetMsgAddDelRootDir(header);
        case AIRPDF_MSG_ID_SETT_ROOT_DIR_LIST:return new CNetMsgRootDirList(header);
        case AIRPDF_MSG_ID_SETT_REQ_MOUNT_POINTS: return new CNetMsgReqMountPoints(header);
        case AIRPDF_MSG_ID_SETT_MOUNT_POINTS: return new CNetMsgMountPoints(header);
    }
    Q_ASSERT(NULL);
    return NULL;
}