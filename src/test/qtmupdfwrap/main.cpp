#include "stdafx.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);

    if (ctx == NULL)
    {
        return 0;
    }

    return a.exec();
}
