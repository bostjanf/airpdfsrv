#include "StdAfx.h"
#include "tcpsrv.h"

#include "netsock.h"
#include "srvbridge.h"


CTcpSrv::CTcpSrv(void)
    :QTcpServer(NULL)
{    
}


CTcpSrv::~CTcpSrv(void)
{    
}

bool CTcpSrv::Start(quint16 nPort)
{
    if (listen(	QHostAddress::Any, nPort))    
    {
        AIRPDF_LOG(LOG_LEVEL_INFO,  QString("Server started on port %1").arg(nPort));
        return true;
    }
    else
    {
        AIRPDF_LOG(LOG_LEVEL_ERROR,  QString("Failed to start server on port %1. %2").arg(nPort).arg(errorString()));
    }
    return false;
}

void CTcpSrv::incomingConnection(int socketDescriptor)
{
    CNetSrvBridge* pBridge = new CNetSrvBridge();
    CNetSock* pSock = new CNetSock(pBridge);//sock is now pointer owner of CNetSrvBridge
    if (pSock->setSocketDescriptor(socketDescriptor))
    {       
        AIRPDF_LOG(LOG_LEVEL_INFO,  QString("Client [%1] connected from %2:%3.").arg(socketDescriptor).arg(pSock->peerAddress().toString()).arg(pSock->peerPort()));
        pBridge->SetNetSock(pSock);
    }
    else
    {
        AIRPDF_LOG(LOG_LEVEL_ERROR,  QString("Error accept client [%1].").arg(socketDescriptor));
        delete pSock;
    }
}
