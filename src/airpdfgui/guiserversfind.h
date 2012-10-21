    

#include "ui_guiserversfind.h"

class CMainWnd;
class CNetIdent;
class CNetMsgSrvVersion;
class CNetMsgSrvInfo;
class CGUIServersFind : public CStackPage
{
    Q_OBJECT
public:
    static QString Os(quint8 nOs);
public:
    CGUIServersFind(CMainWnd *parent);
    ~CGUIServersFind();

    void OnFirstShow();
    void OnShow();

    bool OnMsg(CNetMsgBase* pNetMsg);
    bool OnNetError(QString strTxt);
protected:
    bool OnMsgSrvVersion(CNetMsgSrvVersion* pMsg);
    bool OnMsgSrvInfo(CNetMsgSrvInfo* pMsg);
    void AddToList(QString strIpAddress, QString strHostname);
    bool IsAlreadyInShown(QString strIpAddress);
protected slots:
       void OnHostFound(QString, QString);
       void SendNewBroadCast();
       void ConnectToSrv(QListWidgetItem*);
private:
    Ui::CGUIServersFind ui;
    CNetIdent* m_pNetFinder;
    QTimer     m_broadcastTimer;
};
