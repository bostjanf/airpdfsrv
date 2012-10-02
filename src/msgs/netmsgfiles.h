#pragma once

///////////////////////CNetMsgReqDir////////////////////////////
class CNetMsgReqDir : public CNetMsgBaseBuffered
{
public:
    CNetMsgReqDir(QString strRoot, QString strRelPath);
    CNetMsgReqDir(CNetHeader& header);
    virtual ~CNetMsgReqDir(void); 

    QString RelativeDir();    
    QString Root();    
    QString Path();

protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QString m_strRoot;
    QString m_strRelPath;
};


///////////////////////CNetMsgReqDir////////////////////////////
class CNetMsgDirFiles : public CNetMsgBaseBuffered
{
public:    
    class CItem
    {
    public:
        CItem();
        CItem(QString strNameIn, quint8 nTypeIn, quint32 nSize);
        bool LoadMsg(QDataStream& in);
        bool SaveMsg(QDataStream& out);
    public:
        quint8 nType;
        QString strName;        
        quint32 nSize;
    };

    typedef QList<const CItem*> ConstItemList ;
public:
    CNetMsgDirFiles(QList<CItem> list, bool bIsRootDir);
    CNetMsgDirFiles(CNetHeader& header);
    virtual ~CNetMsgDirFiles(void);     
    bool Items(ConstItemList& list);
    bool IsRootDir();
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:    
    QList<CItem> m_listItems;
    quint8 m_bIsRootDir;
};

