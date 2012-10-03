#pragma once

class CBridgeNet;
class CNetSock;
class CTcpSrv : QTcpServer
{
public:
    CTcpSrv(void);
    virtual ~CTcpSrv(void);
    bool Start(quint16 nPort);
 protected:
     void incomingConnection(int socketDescriptor);
};
