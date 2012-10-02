#include "stdafx.h"
#include "mupdfwrap.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    {
        QDir dir("../../../samples");   
        QString strFilePath =  dir.absoluteFilePath("BlenderCourse.pdf");

        QString strFileOutPath =  QString("Out.png");

        CMuPdfWrap pdfWarp(NULL);
        if (pdfWarp.OpenPdf(strFilePath, "", "") == CMuPdfWrap::RET_CODE_SUCCESS)
        {
            qDebug()<<"Page Count :" << pdfWarp.PageCount();
            pdfWarp.DrawPage(3, false, 1080, strFileOutPath);
        }
    }

    /*
    fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);

    if (ctx == NULL)
    {
        return 0;
    }

    

    fz_set_aa_level(ctx, 8);
    fz_document *doc = NULL;
    fz_try(ctx)
	{
		doc = fz_open_document(ctx, strFilePath.toAscii().data());                 
	}
	fz_catch(ctx)
	{
		 qDebug()<<"Error opening file:" << strFilePath;
         return 0;
	}

    /*
    if (fz_needs_password(doc))
    {
        if (!fz_authenticate_password(doc, password))
        {
            //fz_throw(ctx, "cannot authenticate password: %s", filename);
            return 0;
        }
    }
    */
    /*
    fz_page *page = NULL;
    fz_try(ctx)
	{
		page = fz_load_page(doc, 0);
	}
	fz_catch(ctx)
	{
         qDebug()<<"Error opening file:" << strFilePath;		
	}
   */
  
    return a.exec();
}
