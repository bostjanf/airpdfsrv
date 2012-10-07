#include "stdafx.h"

#include "netident.h"
#include "testbroadcast.h"
#include "testversions.h"
#include "netsocknobridge.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);   
    QString strIp;
    {
        CTestBroadCast test;
        QTest::qExec(&test, argc, argv);
        strIp = test.Ip();
    }
    bool bTestFailed=false;
    if (!strIp.isEmpty())
    {
        CNetSockNoBridge netSock;           
        {//Test connection and versions
            CTestVersions verTest(&netSock, strIp,bTestFailed);
            QTest::qExec(&verTest, argc, argv);
        }
        if (!bTestFailed)
        {
            
        }
    }
    return a.exec();
}
