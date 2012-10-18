#include "StdAfx.h"
#include "stackpage.h"
#include "mainwnd.h"

CStackPage::CStackPage(CMainWnd* pMainWnd)
    :QWidget(pMainWnd->StackWidget()),m_pMainWnd(pMainWnd)
{
}

CStackPage::~CStackPage(void)
{
}

bool CStackPage::OnMsg(CNetMsgBase* pNetMsg)
{
    return false;
}
bool CStackPage::OnNetError(QString strTxt)
{
    return false;
}

void CStackPage::OnFirstShow()
{
    OnShow();
}

void CStackPage::OnShow()
{    
}

void CStackPage::AddPage(CStackPage* page)
{
    m_pMainWnd->AddPage(page);
}
void CStackPage::RemoveThisPage()
{
    m_pMainWnd->RemovePage(this);
}

void CStackPage::SendMsg(CNetMsgBase* pNetMsg)
{
    m_pMainWnd->SendMsg(pNetMsg);
}

