#pragma once
class CAppSettings
{
protected:
    class CItem;
    enum SERVER_SETTING_ITEM_TYPE
    {
        SERVER_SETTING_ITEM_TYPE_SRV_NAME=100,
        SERVER_SETTING_ITEM_TYPE_SRV_PORT,
        SERVER_SETTING_ITEM_TYPE_SRV_DIR_ROOTS,
        SERVER_SETTING_ITEM_TYPE_SRV_PWD,
        SERVER_SETTING_ITEM_TYPE_SRV_GUID,
        SERVER_SETTING_ITEM_TYPE_SRV_FLAGS
    };
public:
    static CAppSettings* Instance();
    static void CleanUp();
protected:
    CAppSettings(void);
    virtual ~CAppSettings(void);
public:
    QString GetSrvName();
    void    SetSrvName(QString);

    void    SetSrvPort(quint16 nPort);
    quint16 GetSrvPort();

    bool  AddRootDir(QString strRoot, QString strPath);
    bool  RemoveRootDir(QString strRoot);
    bool  GetRoots( QHash<QString, QString>& hashRoots);

    void SetPwd(QByteArray arr);
    QByteArray GetPwd();
    bool IsPwdSet();

    QByteArray GetGuid();
    void CheckGuidAtStartup();

    quint64 GetFlags();    
    void    SetFlags(quint64 nFlag);    

protected:
    void CleanHash();
    bool Load();
    bool Save();
    CItem* BuildItem(quint32 nItemType);
protected:
    static CAppSettings* m_pThis;
protected:
    QString m_strFilePath;
    quint32 m_nVersion;
    QHash<quint32, CItem*> m_hash;
protected:
//////////////items definitions//////////////////////////////////////
    class CItem
    {
    public:
        CItem(SERVER_SETTING_ITEM_TYPE typeIn);
        virtual ~CItem();   
        SERVER_SETTING_ITEM_TYPE ItemType();
        
        virtual void Save(QDataStream& out)=0;
        virtual bool Load(QDataStream& in)=0;
    private:
        const SERVER_SETTING_ITEM_TYPE m_type;
    };
//////////////items server name//////////////////////////////////////
    class CServerNameItem : public CItem
    { 
    public:
        CServerNameItem();
        virtual ~CServerNameItem();
        static QString DefaultValue(); 

        void SetSrvName(QString strSrvName);
        QString GetSrvName();

        void Save(QDataStream& out);
        bool Load(QDataStream& in);      
    protected:
        QString m_strSrvName;
    };

    //////////////items server port//////////////////////////////////////
    class CServerPortItem : public CItem
    { 
    public:
        CServerPortItem();
        virtual ~CServerPortItem();
        static quint16 DefaultValue(); 

        void SetSrvPort(quint16 nPort);
        quint16 GetSrvPort();

        void Save(QDataStream& out);
        bool Load(QDataStream& in);      
    protected:
        quint16 m_nPort;
    };
   //////////////items server roots//////////////////////////////////////
    class CServerRootsDirItem : public CItem
    { 
    public:
        CServerRootsDirItem();
        virtual ~CServerRootsDirItem();  

        bool  Add(QString strRoot, QString strPath);
        bool  Remove(QString strRoot);
        bool  GetRoots( QHash<QString, QString>& hashRoots);

        void Save(QDataStream& out);
        bool Load(QDataStream& in);      
    protected:
        QHash<QString, QString> m_hashRoots;
    };

    //////////////item pwd//////////////////////////////////////
    class CPwdItem : public CItem
    { 
    public:
        CPwdItem();
        virtual ~CPwdItem();  

        void SetPwd(QByteArray arr);
        QByteArray GetPwd();

        void Save(QDataStream& out);
        bool Load(QDataStream& in);      
    protected:
        QByteArray  m_arrPwd;
    };

     //////////////item guid//////////////////////////////////////
    class CGuidItem : public CItem
    { 
    public:
        CGuidItem();
        virtual ~CGuidItem();  

        void SetGuid(QByteArray arr);
        QByteArray GetGuid();

        void Save(QDataStream& out);
        bool Load(QDataStream& in);      
    protected:
        QByteArray  m_arrGuid;
    };

     //////////////items server flags//////////////////////////////////////
    class CServerFlagsItem : public CItem
    { 
    public:
        CServerFlagsItem();
        virtual ~CServerFlagsItem();
        static quint64 DefaultValue(); 

        void SetFlags(quint64 nFlags);
        quint64 GetFlags();

        void Save(QDataStream& out);
        bool Load(QDataStream& in);      
    protected:
        quint64 m_nFlags;
    };
};



