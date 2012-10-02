#include "stdafx.h"

#include "netident.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CAirPdfLog::SetLogLevel(CAirPdfLog::LOG_LEVEL_DEBUG);

    //test log outputs
    AIRPDF_LOG(LOG_LEVEL_INFO,      "log enabled");
    AIRPDF_LOG(LOG_LEVEL_WARNING,   "log enabled");
    AIRPDF_LOG(LOG_LEVEL_ERROR,     "log enabled");
    AIRPDF_LOG(LOG_LEVEL_DEBUG,     "log enabled");
    
    AIRPDF_LOG(LOG_LEVEL_INFO,  QString("%1 app started").arg(QFileInfo(argv[0]).absoluteFilePath()));
    
    CNetIdent netIdent(AIRPDF_MSG_ID_BROADCAST_SRV);
    netIdent.Init();
   
    return a.exec();
}
