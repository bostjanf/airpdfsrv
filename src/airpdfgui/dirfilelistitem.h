#pragma once
class CDirFileListItem : public QWidget
{
public:
    CDirFileListItem(QWidget* pParent, quint8 nType, QString strName, quint32 nSize, quint32 nDate);
    virtual ~CDirFileListItem(void);

    quint8 Type();
    QString Name();

    void Selected();
    void UnSelected();
protected:
    void BuildDirUI(QHBoxLayout* horizontalLayout);
    void BuildFileUI(QHBoxLayout* horizontalLayout, quint32 nSize, quint32 nDate);    
    void SetIcon(QLabel* pLabel);
protected:
    quint8 m_nType;
    QString m_strName;
};