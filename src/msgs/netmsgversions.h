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
    quint16 Version(){return m_nVersion;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint16 m_nVersion;
    quint8  m_nOS;     //PROTOCOL_MACHINE_OS_XXX
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
    quint8 Os(){return m_nOS;}
    QString Comp(){return m_strComp;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint16 m_nVersion;
    quint8  m_nOS;     //PROTOCOL_MACHINE_OS_XXX
    QString m_strComp;
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
    QString DevName(){return m_strDevName;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint16 m_nVersion;
    quint8  m_nDev;
    QString m_strDevName;
};


///////////////////////CNetMsgSrvInfo////////////////////////////
class CNetMsgSrvInfo : public CNetMsgBaseBuffered
{
public:
    CNetMsgSrvInfo(quint32 nFlags, QString strInfo);
    CNetMsgSrvInfo(CNetHeader& header);
    virtual ~CNetMsgSrvInfo(void);     
    quint32 Flags(){return m_nFlags;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint32 m_nFlags;
    QString m_strInfo;
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