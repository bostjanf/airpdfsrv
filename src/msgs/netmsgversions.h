#pragma once

///////////////////////CNetMsgSrvVersion////////////////////////////
class CNetMsgSrvVersion : public CNetMsgBaseBuffered
{
public:
    CNetMsgSrvVersion();
    CNetMsgSrvVersion(CNetHeader& header);
    virtual ~CNetMsgSrvVersion(void); 
    quint16 MajorVersion();
    quint16 MinorVersion();
    quint8 Os(){return m_nOS;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint16 m_nVersion;
    quint8  m_nOS;
    quint64 m_nFlags;
    QString m_strGuid;
};


///////////////////////CNetMsgGuiVersion////////////////////////////
class CNetMsgGuiVersion : public CNetMsgBaseBuffered
{
public:
    CNetMsgGuiVersion();
    CNetMsgGuiVersion(CNetHeader& header);
    virtual ~CNetMsgGuiVersion(void); 
    quint16 MajorVersion();
    quint16 MinorVersion();
     
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint16 m_nVersion;
};


///////////////////////CNetMsgIosVersion////////////////////////////
class CNetMsgIosVersion : public CNetMsgBaseBuffered
{
public:
    CNetMsgIosVersion();
    CNetMsgIosVersion(CNetHeader& header);
    virtual ~CNetMsgIosVersion(void); 
    quint16 MajorVersion();
    quint16 MinorVersion();
    quint8 Device(){return m_nDev;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint16 m_nVersion;
    quint8  m_nDev;
};


///////////////////////CNetMsgBookPageError////////////////////////////
class CNetMsgError : public CNetMsgBaseBuffered
{
public:
    CNetMsgError(QString strError);

    CNetMsgError(CNetHeader& header);
    virtual ~CNetMsgError(void); 
    QString Error() {return m_strError;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:  
    QString m_strError;   
};