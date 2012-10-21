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
    bool OnMsgDirFiles(CNetMsgDirFiles* pMsg);
protected slots:
       void OnServers();
       void OnRoots();       
       void OnItemDoubleClicked();
private:
    Ui::CGUIDirFiles ui;
};
