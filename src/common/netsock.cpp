#include "StdAfx.h"
#include "netsock.h"
#include "netmsgbase.h"
#include "ibridge.h"

CNetSock::CNetSock(IBridgeNet* pBridge, QObject *parent)
    : QTcpSocket(parent),m_pMsgOut(NULL),m_pMsgIn(NULL), m_pBridge(pBridge)
{
    
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(SockBytesSend(qint64)));
    connect(this, SIGNAL(readyRead()), this, SLOT(SockReadyRead()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SockError(QAbstractSocket::SocketError)));
}

CNetSock::~CNetSock()
{   
    close();
    if (m_pMsgOut != NULL)
    {
        delete m_pMsgOut;
        m_pMsgOut=NULL;
    }

    if (m_pMsgIn != NULL)
    {
        delete m_pMsgIn;
        m_pMsgIn=NULL;
    }

    while (!m_listOut.isEmpty())
    {
        delete m_listOut.takeFirst();
    }

    if (m_pBridge != NULL)
    {
        delete m_pBridge;
        m_pBridge=NULL;
    }
}

void CNetSock::SendMsg(CNetMsgBase* pMsg)
{
    if (m_pMsgOut != NULL)
    {
        m_listOut.append(pMsg);
    }
    else
    {
        m_pMsgOut = pMsg;
         QByteArray* pArr=NULL;
        if (m_pMsgOut->DataForSend(&pArr) && (pArr!=NULL))
        {
            if (write(*pArr) != pArr->size())
            {
                ForceClose(QString("Socket write error. %1").arg(errorString()));
            }
        }
        else
        {
            ForceClose(QString("Msg [%1] convert for send error").arg(m_pMsgOut->MsgId()));
        }
    }
}

void CNetSock::ForceClose(QString strError)
{
    close();
    m_pBridge->OnError(strError);
}

void CNetSock::SockBytesSend(qint64 nSend)
{
    if (m_pMsgOut)
    {
        if (m_pMsgOut->OnBytesSend(nSend))
        {
            delete m_pMsgOut;            
            if (m_listOut.isEmpty())
            {
                m_pMsgOut = NULL;
                return;
            }
            m_pMsgOut = m_listOut.takeFirst();
        }       
        QByteArray* pArr=NULL;
        if (m_pMsgOut->DataForSend(&pArr) && (pArr!=NULL))
        {
            if (write(*pArr) != pArr->size())
            {
                ForceClose(QString("Socket write error. %1").arg(errorString()));
            }
        }
        else
        {
            ForceClose(QString("Msg [%1] convert for send error").arg(m_pMsgOut->MsgId()));
        }
    }
    else
    {
        ForceClose("Bytes send call on empty outgoing msg");        
    }
}

void CNetSock::SockReadyRead()
{
    quint32 nPos=0;
    quint32 nLeftOver=m_arrLeftOver.size();
    quint32 nLen=bytesAvailable()+nLeftOver;

    if ((m_pMsgIn==NULL) && (nLen < CNetHeader::HEADER_SIZE))
    {
        return;        
    }
    
    QByteArray data;
    data.reserve(nLen);
    if (nLeftOver)
    {
        data.append(m_arrLeftOver);
        data.append(read((nLen-nLeftOver)));
        m_arrLeftOver.clear();
    }
    else
    {
        data.append(read(nLen));
    }

    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);   
    while (nLen > buffer.pos())
    {
        
        if (m_pMsgIn==NULL)
        {
            if (nLen < CNetHeader::HEADER_SIZE)
            {
                quint32 nBuffLeft = nLen-buffer.pos();
                m_arrLeftOver.resize(nBuffLeft);
                m_arrLeftOver.append(buffer.read(nBuffLeft));
                return;
            }
            else
            {
                m_pMsgIn = CNetMsgFactory::CreateMsg(buffer);
                if (m_pMsgIn == NULL)
                {
                   ForceClose("Invalid data received.");
                   return;
                }                
            }
        }
        switch (m_pMsgIn->OnReceivedData(buffer,(nLen-buffer.pos())))
        {
            case 0:
            {
                m_pBridge->OnReceivedMsg(m_pMsgIn);
                m_pMsgIn = NULL;
             }
            break;
            case 1://not all data yet
            {
                Q_ASSERT(nLen==buffer.pos());
                return;
            }break;
            default://error -1
            {
                ForceClose(QString("Invalid data received.%1").arg(m_pMsgIn->MsgId()));
            }
        }
    }
}

void CNetSock::SockError(QAbstractSocket::SocketError err)
{
    ForceClose(errorString());
}