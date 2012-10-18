#include "StdAfx.h"
#include "mainwnd.h"
#include "guiserversfind.h"
#include "netident.h"
#include "netmsgversions.h"


//static
QString CGUIServersFind::Os(quint8 nOs)
{
    switch (nOs)
    {
        case PROTOCOL_MACHINE_OS_WINDOWS: return "Windows";
        case PROTOCOL_MACHINE_OS_OSX: return "OSX";
        case PROTOCOL_MACHINE_OS_IOS: return "IOS";
        case PROTOCOL_MACHINE_OS_LINUX: return "Linux";
        default: return QString("Unknown %1").arg(nOs);
    }
}

CGUIServersFind::CGUIServersFind(CMainWnd *parent)
    : CStackPage(parent),m_broadcastTimer(this)
{
    ui.setupUi(this);
    m_pNetFinder = new CNetIdent(AIRPDF_MSG_ID_BROADCAST_GUI);

    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ConnectToSrv(QListWidgetItem*)));

}

CGUIServersFind::~CGUIServersFind()
{
    if (m_pNetFinder)
    {
        delete m_pNetFinder;
        m_pNetFinder=NULL;
    }
}

void CGUIServersFind::OnFirstShow()
{
    
    if (m_pNetFinder->Init())
    {
        connect(m_pNetFinder, SIGNAL(OnHostFound(QString, QString)), this, SLOT(OnHostFound(QString, QString)));
        connect(&m_broadcastTimer, SIGNAL(timeout()), this, SLOT(SendNewBroadCast()));
        m_pNetFinder->ResetBrodcast();
        m_broadcastTimer.setSingleShot(false);
        m_broadcastTimer.start(2000);
    }
    else
    {            
        QMessageBox::critical(this, "Network error", "Error starting broadcast client.");    
    }

    AddToList("lol", "lol");
}

void CGUIServersFind::OnShow()
{
    ui.listWidget->clear ();//clear

    m_pMainWnd->Disconnect();    
    m_pNetFinder->ResetBrodcast();
    m_broadcastTimer.start(2000);

}

bool CGUIServersFind::OnMsg(CNetMsgBase* pNetMsg)
{
    switch(pNetMsg->MsgId())
    {
        case AIRPDF_MSG_ID_SRV_VERSION:
        {
            return OnMsgSrvVersion((CNetMsgSrvVersion*)pNetMsg);
        }break;
    }
    return false;
}

bool CGUIServersFind::OnNetError(QString strTxt)
{
    ui.labelInfo->setText(strTxt);
    ui.listWidget->setEnabled(true);
    QList<QListWidgetItem *> list= ui.listWidget->selectedItems();
    while (!list.isEmpty())
    {
        list.takeFirst()->setSelected(false);
    }
    m_pNetFinder->ResetBrodcast();
    m_broadcastTimer.start(2000);
    return true;
}


bool CGUIServersFind::OnMsgSrvVersion(CNetMsgSrvVersion* pMsg)
{
    ui.labelInfo->setText(QString("Connected to server version %1.%2 running on %3")
            .arg(pMsg->MajorVersion())
            .arg(pMsg->MinorVersion())
            .arg(Os(pMsg->Os())));

    SendMsg(new CNetMsgGuiVersion());
    return true;
}

void CGUIServersFind::AddToList(QString strIpAddress, QString strHostname)
{
    QIcon icon;
    icon.addFile(":/img/Resources/img/comp_nosel.png", QSize(40,40), QIcon::Selected, QIcon::Off);
    icon.addFile(":/img/Resources/img/comp_sel.png", QSize(40,40), QIcon::Selected, QIcon::On);
    QListWidgetItem* pItem =new QListWidgetItem(icon, strHostname, ui.listWidget);   

    pItem->setToolTip(strIpAddress);
    pItem->setData(Qt::UserRole, QVariant(0)); 

}

////////////slots////////////////////////////////
void CGUIServersFind::OnHostFound(QString strIpAddress, QString strHostname)
{   
    //check if already in the list
    for (quint32 idx=0; idx< ui.listWidget->count(); idx++)
    {
        QListWidgetItem* pItem = ui.listWidget->item(idx);
        if ((pItem->text().compare(strHostname)==0) &&
            (pItem->toolTip().compare(strIpAddress)==0))
        {//reset remove count
            pItem->setData(Qt::UserRole, QVariant(0)); 
            return;
        }
    }
    AddToList(strIpAddress, strHostname);
}


void CGUIServersFind::SendNewBroadCast()
{
    m_pNetFinder->ResetBrodcast();
    //remove timeouts items
    QList<QListWidgetItem *> listRemove;
    for (quint32 idx=0; idx< ui.listWidget->count(); idx++)
    {
        QListWidgetItem* pItem = ui.listWidget->item(idx);        
        QVariant var = pItem->data(Qt::UserRole);
        if (var.isValid() && var.type() == QVariant::Int)
        {
            QString sss = pItem->text();
            qint32 nCount= var.toInt();
            if (nCount < 3)
            {
                 pItem->setData(Qt::UserRole, QVariant(++nCount)); 
            }
            else
            {
                listRemove.append(pItem);
            }
        }
        else
        {
            Q_ASSERT(NULL);
        }

    }

    while (!listRemove.isEmpty())
    {
        QListWidgetItem* pItem = listRemove.takeLast();
        ui.listWidget->removeItemWidget(pItem);
        delete pItem;
    }
}

void CGUIServersFind::ConnectToSrv(QListWidgetItem* pItem)
{
    m_pMainWnd->Disconnect();
    m_pMainWnd->Connect(pItem->toolTip(), AIRPDF_COMMUNICATE_PORT);
    ui.listWidget->setEnabled(false);

    ui.labelInfo->setText(QString("Connecting to server %1").arg(pItem->text()));
    m_broadcastTimer.stop();

}

