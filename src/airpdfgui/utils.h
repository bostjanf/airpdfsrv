#pragma once
class CUtils
{
public:
    static QString SizeHuman(quint32 nSize);

protected:
    static QStringList m_listUnits;
};

