#pragma once

///////////////////////CNetMsgReqBookInfo////////////////////////////
class CNetMsgReqBookInfo : public CNetMsgBaseBuffered
{
public:
    CNetMsgReqBookInfo(QString strRoot, QString strPath);
    CNetMsgReqBookInfo(CNetHeader& header);

    virtual ~CNetMsgReqBookInfo(void); 
    QString Root();
    QString Path();
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QString m_strRoot;
    QString m_strPath;
   
};

///////////////////////CNetMsgBookInfo////////////////////////////
class CNetMsgBookInfo : public CNetMsgBaseBuffered
{
public:
    CNetMsgBookInfo(quint32 nPages);
    CNetMsgBookInfo(QString strError);

    CNetMsgBookInfo(CNetHeader& header);
    virtual ~CNetMsgBookInfo(void); 
    quint32 Pages(){return m_nPages;} 
    QString Error() {return m_strError;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint32 m_nPages;
    QString m_strError;   
};

///////////////////////CNetMsgReqBookPage////////////////////////////
class CNetMsgReqBookPage : public CNetMsgBaseBuffered
{
public:
    CNetMsgReqBookPage(QString strRoot, QString strPath, quint32 nPage, quint16 nDpi, quint8 nFlags);
    CNetMsgReqBookPage(CNetHeader& header);
    virtual ~CNetMsgReqBookPage(void); 
    
    QString Root();
    QString Path();
    quint32 Page();
    quint16 Dpi();
    quint8 Flags();
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QString m_strRoot;
    QString m_strPath;
    quint32 m_nPage;  
    quint16 m_nDpi;
    quint8 m_nFlags;
};


///////////////////////CNetMsgBookPage////////////////////////////
class CNetMsgBookPage : public CNetMsgBase
{
protected:
    static const quint32 SEND_BUFF_SIZE;
public:
    CNetMsgBookPage(QFile* pFile);
    CNetMsgBookPage(CNetHeader& header);
    virtual ~CNetMsgBookPage(void); 
    QByteArray* Data(){return &m_arr;} 
protected:
    qint8 OnReceivedData(QBuffer& buffer, quint32 nSize);
    bool  DataForSend(QByteArray** pData);
    bool  OnBytesSend(qint64 nSend);
protected:
    QByteArray m_arr;      //received data holder
    QFile*     m_pFileToSend;//do not serialize
    quint32    m_nDataMissing;
};

///////////////////////CNetMsgBookCalcDpi////////////////////////////
class CNetMsgBookCalcDpi : public CNetMsgBaseBuffered
{
public:
    CNetMsgBookCalcDpi(QString strRoot, QString strPath, quint32 nPage, quint16 nLargestSize);
    CNetMsgBookCalcDpi(CNetHeader& header);
    virtual ~CNetMsgBookCalcDpi(void); 
    
    QString Root();
    QString Path();
    quint32 Page();
    quint16 LargestSize();
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QString m_strRoot;
    QString m_strPath;
    quint32 m_nPage;  
    quint16 m_nLargestSize;
};

///////////////////////CNetMsgBookDpi////////////////////////////
class CNetMsgBookDpi : public CNetMsgBaseBuffered
{
public:
    CNetMsgBookDpi(quint32 nPage, quint16 nDpi);
    CNetMsgBookDpi(CNetHeader& header);
    virtual ~CNetMsgBookDpi(void); 
    quint32 Page();
    quint16 Dpi();
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:    
    quint32 m_nPage;  
    quint16 m_nDpi;
};
