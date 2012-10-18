#pragma once

#include "ui_mainwnd.h"

class CStackPage;
class CClientBridge;
class CMainWnd : public QMainWindow
{
    Q_OBJECT

public:
    CMainWnd(QWidget *parent=NULL, Qt::WFlags flags=0);
    ~CMainWnd();
    QStackedWidget* StackWidget();
    void AddPage(CStackPage* page);
    void RemovePage(CStackPage* page);

    void Connect(QString strIp, quint16 nPort);
    void Disconnect();
    void SendMsg(CNetMsgBase* pMsg);
    bool OnReceivedMsg(CNetMsgBase* pMsg);
    void OnNetError(QString strTxt);
signals:
    void SigSendMsg(CNetMsgBase* pMsg);
protected slots:
    void OnSendMsg(CNetMsgBase* pMsg);
    void AnimationDone();
protected:
    void showEvent (QShowEvent*);
    void closeEvent (QCloseEvent*);    
private:
    Ui::CMainWnd ui;
    bool m_bShowFirstTime;
    CClientBridge* m_pClientBridge;
////////////inner class for animation done ////////////////////////
    class CAnimationInfo
    {
    public:
        CAnimationInfo(CStackPage* pPage, bool bPageRemove){m_pPage=pPage;m_bPageRemove=bPageRemove;}

        CStackPage* m_pPage;
        bool m_bPageRemove;
    };
    ///list of pending animation and animation in progress 
    QList<CAnimationInfo> m_listAnimations;
};