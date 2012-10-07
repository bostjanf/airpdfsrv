#pragma once

class CNetIdent;
class CTestBroadCast : public QObject
{
    Q_OBJECT
public:
    CTestBroadCast(QObject *parent=NULL);
    ~CTestBroadCast();
    QString Ip();
public slots:
     void OnHostFound(QString strIpAddress, QString strHostname);
private slots:
    void TestBroadCast();
private:
    CNetIdent* m_pNetIdent;
    QString m_strIp;
};


