#pragma once

///////////////////////CNetHeader////////////////////////////
class CNetHeader
{    
public:        
    static const quint32 HEADER_SIZE;
public:
    CNetHeader();
    CNetHeader(quint8 nMsgId);
    virtual ~CNetHeader();

     bool Load(QBuffer& buffer);
     bool Save(QByteArray& arr);
     bool Save(QDataStream& out);
public:
    quint32 m_nMsgLen;
    quint8 m_nMsgId;
    quint8 m_nFlags;
};

///////////////////////CNetMsgBase////////////////////////////
class CNetMsgBase
{
    friend class CNetSock;
public:
    CNetMsgBase(quint8 nMsgId);
    CNetMsgBase(CNetHeader& header);
    virtual ~CNetMsgBase(void);   
    quint16 MsgId();
protected:    
    virtual qint8 OnReceivedData(QBuffer& buffer, quint32 nSize)=0;
    virtual bool  DataForSend(QByteArray** pData)=0;
    virtual bool  OnBytesSend(qint64 nSend)=0;
protected:
    CNetHeader m_header;
};
///////////////////////CNetMsgBaseBuffered////////////////////////////
class CNetMsgBaseBuffered : public CNetMsgBase
{
public:
    CNetMsgBaseBuffered(quint8 nMsgId);
    CNetMsgBaseBuffered(CNetHeader& header);
    virtual ~CNetMsgBaseBuffered(void);  
protected:    
    virtual bool LoadMsg(QDataStream& in)=0;
    virtual bool SaveMsg(QDataStream& out)=0;

    static bool WriteString(QString& str, QDataStream& in);
    static bool ReadString(QString& str, QDataStream& in);

    qint8 OnReceivedData(QBuffer& buffer, quint32 nSize);
    bool  DataForSend(QByteArray** pData);
    bool  OnBytesSend(qint64 nSend);
private:    
    QByteArray* m_pArrData;
};



///////////////////////CNetMsgFactory////////////////////////////
class CNetMsgFactory
{
public:
    static CNetMsgBase* CreateMsg(QBuffer& buffer);
};