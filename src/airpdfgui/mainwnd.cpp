#include "StdAfx.h"
#include "mainwnd.h"

#include "netmsgs.h"
#include "clientbridge.h"

#include "stackpage.h"
#include "guiserversfind.h"



CMainWnd::CMainWnd(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent,Qt::Window),m_bShowFirstTime(true)
{
    ui.setupUi(this);
    m_pClientBridge = new CClientBridge(this);//its auto delete 

    connect(this, SIGNAL(SigSendMsg(CNetMsgBase*)), this, SLOT(OnSendMsg(CNetMsgBase*)) ,Qt::QueuedConnection);
    connect( ui.stackedWidget, SIGNAL(animationFinished()), this , SLOT(AnimationDone()), Qt::QueuedConnection);
}

CMainWnd::~CMainWnd()
{

}

QStackedWidget* CMainWnd::StackWidget()
{
    return ui.stackedWidget;
}


void CMainWnd::showEvent (QShowEvent*)
{
    if (m_bShowFirstTime)
    {        
        CStackPage* pStack = new CGUIServersFind(this);
        AddPage(pStack);
        pStack->OnFirstShow();
        m_bShowFirstTime=false;
    }
}


void CMainWnd::AddPage(CStackPage* page)
{
    int nIdx = ui.stackedWidget->addWidget(page);
    if (nIdx)
    {
        quint32 nPrev = nIdx-1;
        ui.stackedWidget->setCurrentIndex(nPrev);

        ui.stackedWidget->currentWidget()->setEnabled(false);
        page->setEnabled(false);

        ui.stackedWidget->slideInNext();
        m_listAnimations.append(CAnimationInfo(page, false));
    }
}

void CMainWnd::RemovePage(CStackPage* page)
{
    quint32 nSize =  ui.stackedWidget->count();
    if (nSize)
    {
         page->setEnabled(false);
         ui.stackedWidget->slideInPrev();
         m_listAnimations.append(CAnimationInfo(page, true));
    }
}


void CMainWnd::Connect(QString strIp, quint16 nPort)
{
    m_pClientBridge->Connect(strIp, nPort);
}

void CMainWnd::Disconnect()
{
     m_pClientBridge->Disconnect();
}

void CMainWnd::SendMsg(CNetMsgBase* pMsg)
{
    emit SigSendMsg(pMsg);
}

bool CMainWnd::OnReceivedMsg(CNetMsgBase* pMsg)
{
    bool bRet=false;
    CStackPage* page = qobject_cast<CStackPage*>(ui.stackedWidget->currentWidget());
    Q_ASSERT(page);
    if (page)
    {
        bRet = page->OnMsg(pMsg);
        Q_ASSERT(bRet);
    }
   delete pMsg;   
   return bRet;
}

void CMainWnd::OnNetError(QString strTxt)
{
    bool bRet=false;
    CStackPage* page = qobject_cast<CStackPage*>(ui.stackedWidget->currentWidget());
    Q_ASSERT(page);
    QWidget* pParent = this;
    if (page)
    {
        pParent= page;
        bRet = page->OnNetError(strTxt);        
    }

    if (!bRet)
    {        
        QMessageBox::critical(pParent, "Net(not)work", strTxt);
    }
}

void CMainWnd::closeEvent(QCloseEvent*)
{
    m_pClientBridge->Disconnect();
    m_pClientBridge->CleanUp();
}

//////////////////slots/////////////////////////

void CMainWnd::OnSendMsg(CNetMsgBase* pMsg)
{
    m_pClientBridge->SendMsg(pMsg);
}

void CMainWnd::AnimationDone()
{
    if (m_listAnimations.isEmpty())
    {
        Q_ASSERT(NULL);
        return;
    }

    CAnimationInfo animInfo = m_listAnimations.takeFirst();
    if (animInfo.m_bPageRemove)
    {
        ui.stackedWidget->removeWidget(animInfo.m_pPage);
        animInfo.m_pPage->deleteLater();
        CStackPage* pPageCurr = qobject_cast<CStackPage*>(ui.stackedWidget->currentWidget());
        if (pPageCurr)
        {
            pPageCurr->setEnabled(true);
            pPageCurr->OnShow();
        }
    }
    else
    {
        animInfo.m_pPage->setEnabled(true);
        animInfo.m_pPage->OnFirstShow();
    }
}