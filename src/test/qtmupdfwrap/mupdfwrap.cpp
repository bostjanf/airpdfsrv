#include "StdAfx.h"
#include "mupdfwrap.h"

extern "C"
{
    #include "fitz.h"
};

class InternalMuPdfs
{
public:
    InternalMuPdfs();
    virtual ~InternalMuPdfs();
    //interface
    fz_context* Ctx();
    fz_document* Doc(bool bCanBeNull=false);
    fz_colorspace* Colorspace();
    bool OpenDoc(char* szAbsPath, char* szPwd, bool& nPwdErr);    
    void SetGray(bool bGray);
    quint32 PageCount();
protected:
    void FreeDoc();
protected:    
    fz_context*    m_ctx;
    fz_document*   m_doc;
    fz_colorspace* m_colorspace;
    quint32        m_nPageCount;
};


////////////////InternalMuPdfs//////////////////////////////////////////////////
InternalMuPdfs::InternalMuPdfs()
    :m_doc(NULL), m_colorspace(fz_device_rgb),m_nPageCount(0)
{
    m_ctx =  fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    if (m_ctx)
    {
        fz_set_aa_level(m_ctx, 8);
    }
}

InternalMuPdfs::~InternalMuPdfs()
{
    FreeDoc();
    if (m_ctx)
    {
        fz_free_context(m_ctx);
        m_ctx = NULL;
    }
}

fz_context* InternalMuPdfs::Ctx()
{
    return m_ctx;
}

fz_document* InternalMuPdfs::Doc(bool bCanBeNull)
{
    if (!bCanBeNull)
    {
        Q_ASSERT(m_doc);
    }
    return m_doc;
}
fz_colorspace* InternalMuPdfs::Colorspace()
{
     return m_colorspace;
}

quint32 InternalMuPdfs::PageCount()
{
    return m_nPageCount;
}

bool InternalMuPdfs::OpenDoc(char* szAbsPath, char* szPwd, bool& nPwdErr)
{
    FreeDoc();
    nPwdErr=false;
    fz_try(m_ctx)
	{
        m_doc = fz_open_document(m_ctx, szAbsPath);
    }
    fz_catch(m_ctx)
	{
        FreeDoc();
        return false;
	}

    //is pwd protected
    bool bRet=true;
    nPwdErr=true;
    fz_try(m_ctx)
	{
        if (fz_needs_password(m_doc))
        {
            if (!fz_authenticate_password(m_doc, szPwd))
            {
                bRet=false;
            }
        }
    }
    fz_catch(m_ctx)
	{
        bRet = false;
	}


    if (bRet)
    {
        m_nPageCount =  fz_count_pages(m_doc);        
    }
    else
    {
        FreeDoc();
    }
    return bRet;
}

void InternalMuPdfs::SetGray(bool bGray)
{
    if (bGray)
    {
        if ( m_colorspace != fz_device_gray)
        {
            m_colorspace = fz_device_gray;
        }
    }
    else
    {
        if ( m_colorspace != fz_device_rgb)
        {
            m_colorspace = fz_device_rgb;
        }
    }
}

void InternalMuPdfs::FreeDoc()
{
    if (m_doc)
    {       
        fz_close_document(m_doc);
        m_doc=NULL;
    }
    m_nPageCount=0;
}

////////////////CMuPdfWrap////////////////////////////////////////////////////
CMuPdfWrap::CMuPdfWrap(QObject *parent)
    : QObject(parent)
{
    m_pVars = new InternalMuPdfs();
}

CMuPdfWrap::~CMuPdfWrap()
{
    if (m_pVars)
    {
        delete m_pVars;
        m_pVars=NULL;
    }
}

CMuPdfWrap::RET_CODE CMuPdfWrap::OpenPdf(QString strAbsPath, QString strRelPath, QString m_strPwd)
{
    bool bPwdErr=false;  

    if (m_pVars->Doc(true) && 
       (m_strAbsPath.compare(strAbsPath)==0))
    {//reopen same pdf
        return RET_CODE_SUCCESS;
    }

    if (m_pVars->OpenDoc(strAbsPath.toAscii().data(), m_strPwd.toAscii().data(), bPwdErr))
    {
        m_strAbsPath =  strAbsPath;
        m_strRelPath = strRelPath;
        return RET_CODE_SUCCESS;
    }
    //ups error
    m_strAbsPath.clear();
    m_strRelPath.clear();
    if (bPwdErr)
    {
        return RET_CODE_INVALID_PWD;
    }    
    return RET_CODE_ERROR;
}

quint32 CMuPdfWrap::PageCount()
{
    Q_ASSERT(m_pVars->Doc());
    return m_pVars->PageCount();
}

CMuPdfWrap::RET_CODE CMuPdfWrap::DrawPage(quint32 nPageNum, bool bGrayScale, quint16 nWidth)
{
    m_pVars->SetGray(bGrayScale);
    if (!m_pVars->Doc())
    {
        Q_ASSERT(NULL);
        return RET_CODE_ERROR;
    }

    if (nPageNum)
    {
        nPageNum--;//zero idx
    }
    else
    {
        Q_ASSERT(NULL);
        return RET_CODE_OUT_OF_RANGE;
    }

    if (nPageNum>m_pVars->PageCount())
    {
         Q_ASSERT(NULL);
         return RET_CODE_OUT_OF_RANGE;
    }

    fz_page *page=NULL;
    fz_try(m_pVars->Ctx())
	{
		page = fz_load_page(m_pVars->Doc(), nPageNum);
	}
	fz_catch(m_pVars->Ctx())
	{
		return RET_CODE_ERROR;
	}

    fz_display_list *list = NULL;
	fz_device *dev = NULL;	
	fz_cookie cookie = { 0 };

    fz_try(m_pVars->Ctx())
	{

		list = fz_new_display_list(m_pVars->Ctx());
		dev = fz_new_list_device(m_pVars->Ctx(), list);

        fz_run_page(m_pVars->Doc(), page, dev, fz_identity, &cookie);
	}
    fz_always(m_pVars->Ctx())
	{
		fz_free_device(dev);
		dev = NULL;
	}
	fz_catch(m_pVars->Ctx())
	{
		fz_free_display_list(m_pVars->Ctx(), list);
		fz_free_page(m_pVars->Doc(), page);
		return RET_CODE_ERROR;
	}

    float zoom;
    float nDocW;
	fz_matrix ctm;
	fz_rect bounds, bounds2;
	fz_bbox bbox;
	fz_pixmap *pix = NULL;
	int w, h;

	fz_var(pix);
    bounds = fz_bound_page(m_pVars->Doc(), page);
    nDocW = bounds.x1-bounds.x0;
    zoom = nWidth / nDocW;
    ctm = fz_scale(zoom, zoom);
    bounds2 = fz_transform_rect(ctm, bounds);
    bbox = fz_round_rect(bounds2);    

    fz_try(m_pVars->Ctx())
	{
        pix = fz_new_pixmap_with_bbox(m_pVars->Ctx(), m_pVars->Colorspace(), bbox);
        /*
		if (savealpha)
			fz_clear_pixmap(m_pVars->Ctx(), pix);
		else
        */
			fz_clear_pixmap_with_value(m_pVars->Ctx(), pix, 255);

		dev = fz_new_draw_device(m_pVars->Ctx(), pix);
		if (list)
        {
			fz_run_display_list(list, dev, ctm, bbox, &cookie);
        }
		else
        {
			fz_run_page(m_pVars->Doc(), page, dev, ctm, &cookie);
        }
		fz_free_device(dev);

		dev = NULL;
        
        /*
		if (invert)
            fz_invert_pixmap(m_pVars->Ctx(), pix);
		if (gamma_value != 1)
			fz_gamma_pixmap(m_pVars->Ctx(), pix, gamma_value);
		if (savealpha)
			fz_unmultiply_pixmap(m_pVars->Ctx(), pix);
         */   
        char* szFileOut="test.png";
        fz_write_png(m_pVars->Ctx(), pix, szFileOut, 0);			
	}
	fz_always(m_pVars->Ctx())
	{
		fz_free_device(dev);
		dev = NULL;
		fz_drop_pixmap(m_pVars->Ctx(), pix);
	}
	fz_catch(m_pVars->Ctx())
	{
		fz_free_display_list(m_pVars->Ctx(), list);
        fz_free_page(m_pVars->Doc(), page);
        return  RET_CODE_ERROR;
	}
    

    return RET_CODE_SUCCESS;
}


