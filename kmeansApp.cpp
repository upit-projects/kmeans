/***************************************************************
 * Name:      kmeansApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Madalin Grigore-Enescu & Robert Zamfir ()
 * License:   GNU GENERAL PUBLIC LICENSE
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "kmeansApp.h"
#include "kmeansMain.h"

IMPLEMENT_APP(kmeansApp);

bool kmeansApp::OnInit()
{

    kmeansFrame* frame = new kmeansFrame(0L);
    frame->Show();
    return true;

}
