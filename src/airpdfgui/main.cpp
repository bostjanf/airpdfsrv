#include "stdafx.h"
#include "mainwnd.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    {//set global stle sheet
        QFile file(":/qss/Resources/qqs/styleSheet.qss");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            a.setStyleSheet(file.readAll().constData());
            file.close();
        }
    }

    CMainWnd w;
    w.show();
    return a.exec();
}
