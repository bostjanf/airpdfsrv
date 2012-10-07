#include "StdAfx.h"
#include "testversions.h"
#include "netsocknobridge.h"
#include "netmsgs.h"

CTestVersions::CTestVersions(CNetSockNoBridge*pNet, QString strIP, bool &bTestFailed)
    : QObject(NULL),m_pNet(pNet), m_strIP(strIP),m_nMsgGet(0),m_bTestFailed(bTestFailed)
{
    connect (pNet, SIGNAL(OnReceivedMsg(CNetMsgBase*)), this, SLOT(OnReceivedMsg(CNetMsgBase*)));
}

CTestVersions::~CTestVersions()
{

}

bool CTestVersions::OnReceivedMsg(CNetMsgBase* pMsg)
{
    OnMsg(pMsg);    
    return true;
}

void CTestVersions::OnError(QString strTxt)   
{
    QFAIL ( strTxt.toAscii().constData() );
}

void CTestVersions::TestConnect()
{
    m_pNet->connectToHost(m_strIP, AIRPDF_COMMUNICATE_PORT);
    WaitForMsg(AIRPDF_MSG_ID_SRV_VERSION);
    m_bTestFailed = QTest::currentTestFailed();
}

void CTestVersions::TestVersion()
{
    if (m_bTestFailed)return;
    m_pNet->SendMsg(new CNetMsgGuiVersion());
    WaitForMsg(AIRPDF_MSG_ID_SRV_INFO);
    m_bTestFailed = QTest::currentTestFailed();
}

void  CTestVersions::WaitForMsg(quint32 nMsg)
{
    quint32 nIdx=0;
    do
    {
        QTest::qWait(100);       
        if (m_nMsgGet == nMsg)
        {
            return;
        }
    }while (nIdx<10);
    QString strErr = QString("no %1 msg").arg(nMsg);
    QFAIL ( strErr.toAscii().constData());
}

void CTestVersions::OnMsg(CNetMsgBase* pMsgIn)
{
    m_nMsgGet = pMsgIn->MsgId();
    switch (pMsgIn->MsgId())
    {
        case AIRPDF_MSG_ID_SRV_VERSION:
        {
            CNetMsgSrvVersion* pSrvVer =(CNetMsgSrvVersion*)pMsgIn;
            quint16 nVer = PROTOCOL_VERSION;
            QCOMPARE(pSrvVer->Version() , nVer);
        }break;

        case AIRPDF_MSG_ID_SRV_INFO:
        {
              CNetMsgSrvInfo* pSrvInfo =(CNetMsgSrvInfo*)pMsgIn;
              quint32 nFlags=pSrvInfo->Flags();
              if (nFlags&SRV_INFO_FLAG_NO_ROOTS==SRV_INFO_FLAG_NO_ROOTS)
              {
                   QFAIL ("no directory root set yet");
              }
        }break;

        default:
        {       
             QString strErr = QString("unexpected %1 msg").arg(m_nMsgGet);
             QFAIL ( strErr.toAscii().constData());
        }
    }
}