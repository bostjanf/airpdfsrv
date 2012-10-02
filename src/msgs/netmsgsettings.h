#pragma once

///////////////////////CNetMsgSrvVersion////////////////////////////
class CNetMsgSettings : public CNetMsgBaseBuffered
{
public:
    CNetMsgSettings();
    CNetMsgSettings(quint8 nSet, quint64 nFlags, QString strSrvName);
    CNetMsgSettings(CNetHeader& header);
    virtual ~CNetMsgSettings(void); 

    QString SrvName(){return m_strSrvName;}
    quint64 Flags(){return m_nFlags;}
    quint8 SetSettings(){return m_nSet;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    quint8  m_nSet;
    quint64 m_nFlags;
    QString m_strSrvName;
};


///////////////////////CNetMsgAddDelRootDir////////////////////////////
class CNetMsgAddDelRootDir : public CNetMsgBaseBuffered
{
public:
    CNetMsgAddDelRootDir();
    CNetMsgAddDelRootDir(quint8  nStatus, QString strRootName, QString strRootPath);
    CNetMsgAddDelRootDir(CNetHeader& header);
    virtual ~CNetMsgAddDelRootDir(void); 

    quint8 Status(){return (m_nStatus);}//0=Get list, 1=Add 2=Del
    QString RootName(){return m_strRootName;}   
    QString RootPath(){return m_strRootPath;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
   quint8  m_nStatus;
   QString m_strRootName;
   QString m_strRootPath;
};

///////////////////////CNetMsgRootDirList////////////////////////////
class CNetMsgRootDirList : public CNetMsgBaseBuffered
{
public:
    class CItem
    {
    public:
        CItem();
        CItem(QString strRoot, QString strPath);
        virtual ~CItem();
        QString GetRoot() {return m_strRoot;}
        QString GetPath() {return m_strPath;}
        bool Load(QDataStream& in);
        bool Save(QDataStream& out);
    protected:
        QString m_strRoot;
        QString m_strPath;
    };
public:
    CNetMsgRootDirList();
    CNetMsgRootDirList(QList<CItem> list);
    CNetMsgRootDirList(CNetHeader& header);
    virtual ~CNetMsgRootDirList(void); 
    bool GetList(QList<CItem>& list);
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QList<CItem> m_list;
};


///////////////////////CNetMsgReqMountPoints////////////////////////////
class CNetMsgReqMountPoints : public CNetMsgBaseBuffered
{
public:
    CNetMsgReqMountPoints();
    CNetMsgReqMountPoints(QString strPath);
    CNetMsgReqMountPoints(CNetHeader& header);
    virtual ~CNetMsgReqMountPoints(void); 

    QString Path(){return m_strPath;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QString m_strPath;
};

///////////////////////CNetMsgMountPoints////////////////////////////
class CNetMsgMountPoints : public CNetMsgBaseBuffered
{
public:
    CNetMsgMountPoints();
    CNetMsgMountPoints(QString strPath, QStringList listPaths);
    CNetMsgMountPoints(CNetHeader& header);
    virtual ~CNetMsgMountPoints(void); 

    QString Path(){return m_strPath;}
    QStringList Paths(){return m_listPaths;}
protected:
    bool LoadMsg(QDataStream& in);
    bool SaveMsg(QDataStream& out);
protected:
    QString     m_strPath;
    QStringList m_listPaths;
};



