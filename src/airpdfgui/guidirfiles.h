#pragma once
#include "ui_guidirfiles.h"

class CNetMsgDirFiles;
class CGUIDirFiles : public CStackPage
{
    Q_OBJECT

public:
    CGUIDirFiles(CMainWnd *parent);
    ~CGUIDirFiles();

    void OnFirstShow();
    void OnShow();

    bool OnMsg(CNetMsgBase* pNetMsg);
    bool OnNetError(QString strTxt);
protected:
    void SendReqDirFiles();
    bool OnMsgDirFiles(CNetMsgDirFiles* pMsg);
protected slots:
       void OnServers();
       void OnRoots();       
       void OnItemSelected(QListWidgetItem*);
private:
    Ui::CGUIDirFiles ui;
    QString m_strRoot;
    QString m_strPath;
};
