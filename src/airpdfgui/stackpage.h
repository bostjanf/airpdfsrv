#pragma once

class CMainWnd;
class CStackPage : public QWidget
{
    Q_OBJECT
public:
    CStackPage(CMainWnd* pMainWnd);
    virtual ~CStackPage(void);

    virtual bool OnMsg(CNetMsgBase* pNetMsg)=0;
    virtual bool OnNetError(QString strTxt)=0;
    virtual void OnFirstShow();
    virtual void OnShow()=0;
protected:
     void AddPage(CStackPage*);
     void RemoveThisPage();
     void SendMsg(CNetMsgBase* pNetMsg);
protected:
    CMainWnd* m_pMainWnd;
};

