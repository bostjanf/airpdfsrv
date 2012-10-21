#include "StdAfx.h"
#include "guidirfiles.h"
#include "netmsgfiles.h"
#include "utils.h"
#include "dirfilelistitem.h"

CGUIDirFiles::CGUIDirFiles(CMainWnd *parent)
    : CStackPage(parent)
{
    ui.setupUi(this);

    connect(ui.pushButtonServers, SIGNAL(clicked()), this, SLOT(OnServers()));
    connect(ui.pushButtonRoots, SIGNAL(clicked()), this, SLOT(OnRoots()));
}

CGUIDirFiles::~CGUIDirFiles()
{

}


void CGUIDirFiles::OnFirstShow()
{
    SendMsg(new CNetMsgReqDir("", ""));
}

void CGUIDirFiles::OnShow()
{
}

bool CGUIDirFiles::OnMsg(CNetMsgBase* pNetMsg)
{
    switch(pNetMsg->MsgId())
    {
        case AIRPDF_MSG_ID_DIR_FILES:
        {
            return OnMsgDirFiles((CNetMsgDirFiles*)pNetMsg);
        }break;
    }

    Q_ASSERT(NULL);
    return false;
}

bool CGUIDirFiles::OnNetError(QString strTxt)
{
    
    return false;
}

bool CGUIDirFiles::OnMsgDirFiles(CNetMsgDirFiles* pMsg)
{
    CNetMsgDirFiles::ConstItemList list;
    ui.listWidget->clear();

    if (pMsg->Items(list))
    {
        while (!list.isEmpty())
        {
            const CNetMsgDirFiles::CItem* pItem = list.takeFirst();
            QListWidgetItem* pListItem =new QListWidgetItem();
            pListItem->setSizeHint(QSize(100, 70));
            ui.listWidget->addItem(pListItem);
            ui.listWidget->setItemWidget(pListItem, new CDirFileListItem(ui.listWidget,pItem->nType,pItem->strName,pItem->nSize,0));
        }
    }
     
    return true;
}

//slots/////////////////////////////////////
void CGUIDirFiles::OnServers()
{
    RemoveThisPage();
}

void CGUIDirFiles::OnRoots()
{

}

void CGUIDirFiles::OnItemDoubleClicked()
{
}
