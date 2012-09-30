#pragma once


class InternalMuPdfs;
class CMuPdfWrap : public QObject
{
    Q_OBJECT
public:
    enum RET_CODE
    {
        RET_CODE_SUCCESS=0,
        RET_CODE_ERROR,
        RET_CODE_INVALID_PWD,        
        RET_CODE_OUT_OF_RANGE
    };
public:
    CMuPdfWrap(QObject *parent);
    ~CMuPdfWrap();

    RET_CODE OpenPdf(QString strAbsPath, QString strRelPath, QString m_strPwd);
    quint32 PageCount();
    RET_CODE DrawPage(quint32 nPageNum, bool bGrayScale, quint16 nWidth);
protected:
    InternalMuPdfs* m_pVars;

    //open Doc
    QString m_strAbsPath;
    QString m_strRelPath;
};

