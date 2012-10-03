#pragma once

class CNetMsgBase;
class IBridgeNet
{

public:
    virtual      ~IBridgeNet(void){};
    virtual bool OnReceivedMsg(CNetMsgBase* pMsg)=0;
    virtual bool OnError(QString strTxt)=0;
protected:
    virtual bool SendMsg(CNetMsgBase* pMsg)=0;
};