#include "StdAfx.h"
#include "netident.h"
#include "appsettings.h"


CNetIdent::CNetIdent(quint32 nStartAs)
    : QObject(NULL), m_nStartAs(nStartAs),m_pSocketArr(NULL),m_nNumOfSockets(0),m_bLocalAddressEmited(false)
{    
    m_strHostname =   QHostInfo::localHostName(); 
#ifdef AIR_PDF_SRV
    m_strHostname = CAppSettings::Instance()->GetSrvName();
#endif
    if (m_strHostname.size()>255)
    {
        m_strHostname = m_strHostname.left(255);
    }
}

CNetIdent::~CNetIdent()
{    
    m_nNumOfSockets=0;
    if (m_pSocketArr)
    {
        delete [] m_pSocketArr;
        m_pSocketArr = NULL;
    }
}

bool CNetIdent::Init()
{
    switch(m_nStartAs)
    {
        case AIRPDF_MSG_ID_BROADCAST_SRV:
        {
            m_nNumOfSockets = 1;
            if (m_pSocketArr){delete [] m_pSocketArr;}
            
            m_pSocketArr = new QUdpSocket[m_nNumOfSockets];


            quint16 nPort=AIRPDF_COMMUNICATE_PORT;
#ifdef AIR_PDF_SRV
            nPort = CAppSettings::Instance()->GetSrvPort();
#endif
            if (m_pSocketArr[0].bind(QHostAddress::Any,nPort))
            {
                AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("broadcast server bind to port %1").arg(nPort));
                connect(&m_pSocketArr[0], SIGNAL(readyRead()), this, SLOT(OnDatagram()));
                return true;
            }
            Q_ASSERT(NULL);
            AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Error bind broadcast server to port %1.%2").arg(nPort).arg(m_pSocketArr[0].errorString()));
            return false;
        }
        case AIRPDF_MSG_ID_BROADCAST_GUI:
        {
            m_listBroadcastAdd.clear();
            {//load valid broadcast address                          
                QList<QNetworkInterface> listif= QNetworkInterface::allInterfaces();                
                quint32 nMask=QNetworkInterface::IsUp|QNetworkInterface::IsRunning|QNetworkInterface::CanBroadcast;
                for (quint32 idx=0; idx<listif.size(); idx++)
                {
                    QNetworkInterface netIf = listif[idx];
                    if ((netIf.flags()&nMask)!=nMask)
                    {
                        continue;
                    }
                    QList<QNetworkAddressEntry> listAdd =	netIf.addressEntries();
                    for (quint32 idy=0; idy<listAdd.size(); idy++)
                    {
                        QNetworkAddressEntry addEntry = listAdd[idy];
                        if (addEntry.ip().protocol()==QAbstractSocket::IPv4Protocol)
                        {
                            m_listBroadcastAdd.append(addEntry);
                        }
                    }//for netif addresses
                }//network interfaces
            }//load valid interfaces
            if (m_listBroadcastAdd.isEmpty()){Q_ASSERT(NULL);return false;}
            m_nNumOfSockets = m_listBroadcastAdd.size();
            if (m_pSocketArr){delete [] m_pSocketArr;}

            m_pSocketArr = new QUdpSocket[m_nNumOfSockets];

            for (quint32 idx=0; idx<m_nNumOfSockets; idx++)
            {
                quint16 nPort = 60536;
                QNetworkAddressEntry netAdd=m_listBroadcastAdd[idx];
                while (true)
                {
                    if (m_pSocketArr[idx].bind(netAdd.ip(), nPort))
                    {
                        AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("bind broadcast socket %1:%2  ").arg(netAdd.ip().toString()).arg(nPort));
                        connect(&m_pSocketArr[idx], SIGNAL(readyRead()), this, SLOT(OnDatagram()));
                        break;//step out from while loop -> socket bind OK
                    }
                    nPort--;
                    if (nPort < 60336){Q_ASSERT(NULL);return false;}//if none of ports is OK just give up
                }//while try to bind
            }//for valid IP addresses
            BroadCast();

            return true;
        }
        default:
            return false;
    }
}

void CNetIdent::OnDatagram()
{
    QUdpSocket* pSock = qobject_cast<QUdpSocket*>(sender());
    if (pSock == NULL){return;}
    while (pSock->hasPendingDatagrams()) 
    {
         QByteArray datagram;
         datagram.resize(pSock->pendingDatagramSize());

         QHostAddress senderAdd;
         quint16 senderPort;

         pSock->readDatagram(datagram.data(), datagram.size(),
                             &senderAdd, &senderPort);

         if (datagram.size()>9)
         {
             
             quint32 nMsg=0;
             QString strHostName;

             if (!ReadDatagram(&datagram, nMsg, strHostName))
             {
                 AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Invalid broadcast message from %1:%2").arg(senderAdd.toString()).arg(senderPort));
                 Q_ASSERT(NULL);
                 continue;
             }
             if (nMsg == m_nStartAs)
             {
                 continue;
             }

             switch (nMsg)
             {
                case AIRPDF_MSG_ID_BROADCAST_SRV:
                {
                    if (IsLocalAddress(senderAdd.toIPv4Address()))
                    {
                        if (m_bLocalAddressEmited)
                        {
                            AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("Ignore message server running on  %1").arg(senderAdd.toString()));
                        }
                        else
                        {
                            m_bLocalAddressEmited=true;
                            AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("Found server running on  %1").arg(senderAdd.toString()));  
                            emit OnHostFound(senderAdd.toString(), strHostName);
                        }
                    }
                    else
                    {
                        AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("Found server running on  %1").arg(senderAdd.toString()));  
                        emit OnHostFound(senderAdd.toString(), strHostName);
                    }
                }break;

                case AIRPDF_MSG_ID_BROADCAST_GUI:
                case AIRPDF_MSG_ID_BROADCAST_IOS:
                {
                    AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("Server got broadcast msg %1 from %2:%3[%4]").arg(nMsg).arg(senderAdd.toString()).arg(senderPort).arg(strHostName));
                    SendBack(pSock, senderAdd, senderPort);
                }break;
                default:
                {
                    Q_ASSERT(NULL);
                }
             }
         }//if
     }//while
}

bool CNetIdent::IsLocalAddress(quint32 nIp4Add)
{
    for (quint32 idx=0; idx < m_listBroadcastAdd.size(); idx++)
    {
        if (m_listBroadcastAdd[idx].ip().toIPv4Address() == nIp4Add)
        {
            return true;
        }
    }
    return false;
}

bool CNetIdent::BroadCast()
{    
    QByteArray datagram;
    BuildDatagram(datagram);    

    for (quint32 idx=0; idx<m_nNumOfSockets; idx++)
    {
        QHostAddress add =  m_listBroadcastAdd[idx].broadcast();
        if (add.isNull())
        {
            add = QHostAddress::Broadcast;
        }
        quint16 nPort=AIRPDF_COMMUNICATE_PORT;
#ifdef AIR_PDF_SRV
        nPort = CAppSettings::Instance()->GetSrvPort();
#endif
        if (m_pSocketArr[idx].writeDatagram(datagram.data(), datagram.size(), add, nPort)!= datagram.size())
        {
            //Q_ASSERT(NULL);
            AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Failed to send broadcast msg  from %1 with broadcast  mask %2.%3")
                                                                            .arg( m_listBroadcastAdd[idx].ip().toString())
                                                                            .arg( m_listBroadcastAdd[idx].broadcast().toString())
                                                                            .arg( m_pSocketArr[idx].errorString()));
        }
        else
        {
            AIRPDF_LOG(LOG_LEVEL_DEBUG, QString("Sent broadcast msg from %1  with broadcast  mask %2.")
                                                                            .arg( m_listBroadcastAdd[idx].ip().toString())
                                                                            .arg(m_listBroadcastAdd[idx].broadcast().toString()));  
        }
    }
    return true;
}

 bool CNetIdent::ResetBrodcast()
 {
      m_bLocalAddressEmited=false;
      return BroadCast();
 }


bool CNetIdent::SendBack(QUdpSocket*pSock, QHostAddress& add, quint16 nPort)
{
    QByteArray datagram;
    BuildDatagram(datagram);    
    return pSock->writeDatagram(datagram.data(), datagram.size(), add, nPort) ==  datagram.size();   
}

bool CNetIdent::ReadDatagram(QByteArray* pDatagram, quint32& nMsg, QString& strHostName)
{
    quint32 nMagic=0;
    quint16 nHostnameLen =0;

    nMsg = 0;
    strHostName.clear();

    
    QBuffer buffer(pDatagram);

    buffer.open(QIODevice::ReadOnly);             
    buffer.read((char*)&nMagic, sizeof(nMagic));
    if (nMagic != AIRPDF_BROADCAST_MAGIC)
    {
        AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Invalid broadcast msg magic[%1].").arg( nMagic));
        Q_ASSERT(NULL);
        false;
    }
    buffer.read((char*)&nMsg, sizeof(nMsg));
    if (nMsg >= AIRPDF_MSG_ID_SRV_VERSION)
    {
        AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Invalid broadcast msg id[%1].").arg( nMsg));
        return false;       
    }
    buffer.read((char*)&nHostnameLen, sizeof(nHostnameLen));
    if (nHostnameLen > (pDatagram->size()-10))
    {
        AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Invalid broadcast msg hostname length [%1].").arg(nHostnameLen));
        Q_ASSERT(NULL);
        return false;  
    }

    if (nHostnameLen)
    {
        QByteArray arr = buffer.read(nHostnameLen);
        if (arr.size() != nHostnameLen)
        {
            AIRPDF_LOG(LOG_LEVEL_ERROR, QString("Invalid broadcast msg hostname length mismatch [%1]!=[%2].").arg(arr.size()).arg(nHostnameLen));
            Q_ASSERT(NULL);
            return false; 
        }
        strHostName = QString::fromUtf8(arr.constData(),  (nHostnameLen-1));//null char
    }
    return true;
    
}


void CNetIdent::BuildDatagram(QByteArray& datagram)
{ 
    QByteArray arrHostUtf8 = m_strHostname.toUtf8();
    quint16 nHostnameLen = arrHostUtf8.size()+1;//null char  
    datagram.reserve(nHostnameLen+10);
    {
        QBuffer buffer(&datagram);
        buffer.open(QIODevice::WriteOnly);
        quint32 nMagic=AIRPDF_BROADCAST_MAGIC;
        buffer.write((const char*)&nMagic, sizeof(nMagic));
        buffer.write((const char*)&m_nStartAs, sizeof(m_nStartAs));
        //write hostname
        buffer.write((const char*)&nHostnameLen, sizeof(nHostnameLen));
        if (nHostnameLen)
        {
            buffer.write(arrHostUtf8.constData(),nHostnameLen);
            char chNull=0;
            buffer.write(&chNull,sizeof(chNull)); 
        }
    }
}