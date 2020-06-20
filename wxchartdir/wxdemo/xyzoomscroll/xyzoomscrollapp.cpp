/*
** Name:        xyzoomscrollapp.cpp
** Purpose:     XY Zooming and Scrolling
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "xyzoomscrollapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( XyZoomScrollApp )

/*
 * XyZoomScrollApp type definition
 */

IMPLEMENT_CLASS( XyZoomScrollApp, wxApp )

/*
 * XyZoomScrollApp event table definition
 */

BEGIN_EVENT_TABLE( XyZoomScrollApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for XyZoomScrollApp
 */

XyZoomScrollApp::XyZoomScrollApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
XyZoomScrollApp::Init()
{
}

/*
 * Initialisation for XyZoomScrollApp
 */

bool
XyZoomScrollApp::OnInit()
{    
#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	XyZoomScroll* mainWindow = new XyZoomScroll(NULL);
	mainWindow->Show(true);
////@end XyZoomScrollApp initialisation

  return true;
}

/*
 * Cleanup for XyZoomScrollApp
 */

int
XyZoomScrollApp::OnExit()
{    
	return wxApp::OnExit();
}
