#pragma once
class CAirPdfLog
{
public:
    enum LOG_LEVEL
    {
        LOG_LEVEL_DISABLED=0,
        LOG_LEVEL_INFO,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_DEBUG
    };
public:
    static bool LoggingOn(LOG_LEVEL level);
    static bool Log(LOG_LEVEL level, QString str);
    static void SetLogLevel(LOG_LEVEL level);
protected:
    CAirPdfLog(void);
    virtual ~CAirPdfLog(void);
    bool LogIt(LOG_LEVEL level, QString str);
    QString  LogStr(LOG_LEVEL level, QString str);
protected:
    static quint32 m_nLogingLevel;
    static CAirPdfLog* m_pThis;
    QMutex m_mutex;
};

//#define SRV_LOG(level, txt) CAirPdfLog::Log(CAirPdfLog::level, txt)
//#define IS_LOG_ON(level) CAirPdfLog::LoggingOn(CAirPdfLog::level)
#define AIRPDF_LOG(level, txt){if( CAirPdfLog::LoggingOn(CAirPdfLog::level)){CAirPdfLog::Log(CAirPdfLog::level, txt);}}