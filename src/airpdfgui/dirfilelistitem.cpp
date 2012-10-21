#include "StdAfx.h"
#include "dirfilelistitem.h"
#include "utils.h"


CDirFileListItem::CDirFileListItem(QWidget* pParent, quint8 nType, QString strName, quint32 nSize, quint32 nDate)
    :QWidget(pParent),m_nType(nType), m_strName(strName)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setMinimumSize(QSize(100, 40));
    setMaximumSize(QSize(16777215, 70));
    setBaseSize(QSize(100, 40));

    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    BuildDirUI(horizontalLayout);
    if (nType == DIR_FILE_TYPE_FILE)
    {
        BuildFileUI(horizontalLayout,nSize, nDate);
    }
}

CDirFileListItem::~CDirFileListItem(void)
{
}

quint8 CDirFileListItem::Type()
{
    return m_nType;
}

QString CDirFileListItem::Name()
{
    return m_strName;
}


void CDirFileListItem::BuildDirUI(QHBoxLayout* horizontalLayout)
{
    QLabel *labelIcon;
    QLabel *labelTitle;
    
    labelIcon = new QLabel(this);
    SetIcon(labelIcon);
    horizontalLayout->addWidget(labelIcon);

    labelTitle = new QLabel(this);
    labelTitle->setText(m_strName);
    horizontalLayout->addWidget(labelTitle);

    horizontalLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void CDirFileListItem::BuildFileUI(QHBoxLayout* horizontalLayout, quint32 nSize, quint32 nDate)
{            
    QVBoxLayout *verticalLayout;
    QLabel *labelSize;
    QLabel *labelDate;


    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    labelSize = new QLabel(this);
    labelSize->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
    labelSize->setProperty("myFontSize", QVariant(QString::fromUtf8("small")));
    labelSize->setText(CUtils::SizeHuman(nSize));
    verticalLayout->addWidget(labelSize);

    labelDate = new QLabel(this);
    labelDate->setAlignment(Qt::AlignBottom|Qt::AlignRight|Qt::AlignTrailing);
    labelDate->setProperty("myFontSize", QVariant(QString::fromUtf8("small")));
    labelDate->setText(QDateTime::currentDateTime().toString());
    verticalLayout->addWidget(labelDate);
    horizontalLayout->addLayout(verticalLayout);
}

void CDirFileListItem::SetIcon(QLabel* pLabel)
{
    switch (m_nType)        
    {
        case DIR_FILE_TYPE_ROOT:
        case DIR_FILE_TYPE_DIRECTORY:
        {
            pLabel->setPixmap(QPixmap(":/img/Resources/img/folder_nosel.png"));
        }break;
        case DIR_FILE_TYPE_FILE:
        {
            pLabel->setPixmap(QPixmap(":/img/Resources/img/pdf_nosel.png"));
        }break;
        case DIR_FILE_TYPE_DIR_UP:
        {
            pLabel->setPixmap(QPixmap(":/img/Resources/img/folderup_nosel.png"));
        }break;
        default:
            Q_ASSERT(NULL);
    }
}
