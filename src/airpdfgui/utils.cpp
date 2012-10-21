#include "StdAfx.h"
#include "utils.h"

QStringList CUtils::m_listUnits;



QString CUtils::SizeHuman(quint32 nSize)
{   
    if (m_listUnits.isEmpty())
    {
         m_listUnits << "KB" << "MB" << "GB" << "TB";
    }

    double nNum=nSize;
    QStringListIterator it(m_listUnits);
    QString strUnit("bytes");
    while(nNum >= 1024.0 && it.hasNext())
    {
        strUnit = it.next();
        nNum /= 1024.0;
    }
    return QString().setNum(nNum,'g',2)+" "+strUnit;
}