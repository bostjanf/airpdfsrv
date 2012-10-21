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
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemSelected(QListWidgetItem*)));
}

CGUIDirFiles::~CGUIDirFiles()
{

}


void CGUIDirFiles::OnFirstShow()
{
    m_strRoot = "";
    m_strPath = "";
    SendReqDirFiles();
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

void CGUIDirFiles::SendReqDirFiles()
{
    SendMsg(new CNetMsgReqDir(m_strRoot, m_strPath));
    setEnabled(false);
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
    setEnabled(true);
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

void CGUIDirFiles::OnItemSelected(QListWidgetItem* pItem)
{
    QWidget* pWidget=ui.listWidget->itemWidget(pItem);    
    CDirFileListItem* pFileListItem = qobject_cast<CDirFileListItem*>(pWidget);
    if (pFileListItem == NULL)       
    {
        Q_ASSERT(NULL);
        ui.listWidget->clearSelection();
        return;
    }

    switch (pFileListItem->Type())
    {
        case  DIR_FILE_TYPE_ROOT:
        {
            m_strRoot = pFileListItem->Name();
            m_strPath="";
            SendReqDirFiles();
        }break;

        case DIR_FILE_TYPE_DIRECTORY:
        {
             m_strPath+="/"+pFileListItem->Name();
             SendReqDirFiles();
        }break;
        case DIR_FILE_TYPE_FILE:
        case DIR_FILE_TYPE_DIR_UP:
            ui.listWidget->clearSelection();
        break;
        default:
            Q_ASSERT(NULL);
    }
}
