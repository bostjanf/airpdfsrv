#include "StdAfx.h"
#include "testbroadcast.h"
#include "netident.h"

CTestBroadCast::CTestBroadCast(QObject *parent)
    : QObject(parent)
{
    m_pNetIdent = new CNetIdent(AIRPDF_MSG_ID_BROADCAST_GUI);
    connect(m_pNetIdent, SIGNAL(OnHostFound(QString, QString )), this, SLOT(OnHostFound(QString, QString)));
}

CTestBroadCast::~CTestBroadCast()
{
    delete m_pNetIdent;
    m_pNetIdent=NULL;
}

QString CTestBroadCast::Ip()
{
    return m_strIp;
}

void CTestBroadCast::OnHostFound(QString strIpAddress, QString strHostname)
{
    m_strIp = strIpAddress;
}

void CTestBroadCast::TestBroadCast()
{
    quint32 nIdx=0;
    QVERIFY(m_pNetIdent->Init());
    do
    {
        QTest::qWait(100);
        if (!m_strIp.isEmpty())
        {
            return;
        }
        nIdx++;
    }while (nIdx<10);
    QFAIL ( "no broadcast msg" );
}