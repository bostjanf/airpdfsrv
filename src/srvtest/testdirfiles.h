#pragma once 

class CTestDirFiles : public QObject
{
    Q_OBJECT

public:
    CTestDirFiles(bool &bTestFailed);
    ~CTestDirFiles();
public slots:
    bool OnReceivedMsg(CNetMsgBase* pMsg);
    void OnError(QString strTxt);   
private slots:
    void TestAllDirs();
protected:
    void WaitForMsg(quint32 nMsg);
    void OnMsg(CNetMsgBase* pMsgIn);
protected:
    QStringList m_listRoots;
    QStringList m_listCurrentRootDir;
    QString m_strCurrRoot;
};



