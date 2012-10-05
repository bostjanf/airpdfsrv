#pragma once

class CNetMsgBase;
class IBridgeNet
{

public:
    virtual      ~IBridgeNet(void){};
    virtual bool OnReceivedMsg(CNetMsgBase* pMsg)=0;
    virtual void OnError(QString strTxt)=0;
    virtual void SendMsg(CNetMsgBase* pMsg)=0;
};