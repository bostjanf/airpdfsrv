#include "StdAfx.h"
#include "airpdflog.h"

quint32 CAirPdfLog::m_nLogingLevel=LOG_LEVEL_DISABLED;
CAirPdfLog* CAirPdfLog::m_pThis=NULL;

bool CAirPdfLog::LoggingOn(CAirPdfLog::LOG_LEVEL level)
{
    if (level<= m_nLogingLevel)
    {
        return true;
    }
    return false;
}

bool CAirPdfLog::Log(CAirPdfLog::LOG_LEVEL level, QString str)
{
    if (LoggingOn(level))
    {
        if (m_pThis == NULL)
        {
            m_pThis = new CAirPdfLog();
        }
        return m_pThis->LogIt(level, str);
    }
    else
    {
        return false;
    }
}

void CAirPdfLog::SetLogLevel(CAirPdfLog::LOG_LEVEL level)
{
    m_nLogingLevel = level;
}


CAirPdfLog::CAirPdfLog(void)
{
}


CAirPdfLog::~CAirPdfLog(void)
{
}

bool CAirPdfLog::LogIt(LOG_LEVEL level, QString str)
{
    QDateTime time = QDateTime::currentDateTime();
    QMutexLocker locker(&m_mutex);

    qDebug(LogStr(level, str).toAscii().constData());
    return true;
}

QString  CAirPdfLog::LogStr(LOG_LEVEL level, QString str)
{
    QString levelStr("unknown");
    switch (level)
    {
        case LOG_LEVEL_INFO:    levelStr="info   ";break;
        case LOG_LEVEL_ERROR:   levelStr="error  ";break;
        case LOG_LEVEL_WARNING: levelStr="warning";break;
        case LOG_LEVEL_DEBUG:   levelStr="debug  ";break;                                   
    }

    QDateTime time = QDateTime::currentDateTime();
    return QString("%1 %2 %3")
                .arg(time.toString("dd.MMM hh:mm:ss"))
                .arg(levelStr)
                .arg(str);
}