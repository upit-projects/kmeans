/***************************************************************
 * Name:      kmeansMain.h
 * Purpose:   Defines Application Frame
 * Author:    Madalin Grigore-Enescu & Robert Zamfir ()
 **************************************************************/

#ifndef KMEANSMAIN_H
#define KMEANSMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/quantize.h>
#include <wx/grid.h>
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/stattext.h>

#include <vector>
#include "kmeansParams.h"
#include "kmeansPoint.h"
#include "kmeansKmeans.h"

class kmeansFrame: public wxFrame
{
public:

	// Clasa care implementeaza algoritmul k-means
	kmeansKmeans kmeans;

	int algorithmStep = 0;

	std::vector<kmeansPoint> points;

	wxImage loadedImages [100];
	wxImage loadedImagesThumb [100];
	int loadedImagesPersons [100];

	// Calea catre baza de date cu imagini
	wxString currentImageDatabasePath;

	// Params
	kmeansParams params;

	// Elemente grafice
	wxPanel * paramsPanel = nullptr;
	wxPanel * graphPanel = nullptr;
	wxPanel * imagePanel = nullptr;

	wxChartViewer * chartViewerClusters = nullptr;
	wxChartViewer * chartViewerTesting  = nullptr;

	wxBoxSizer * sizerVertical;
	wxBoxSizer * sizerHorizontal;
	wxGrid * kmeansClustersGrid;
	wxButton * openImageDatabaseButton;
	wxButton * runAlgorithmButton;
	wxStaticText * runAlgorithmStepText;

	wxSpinCtrl * inputTextQuantization;
	wxCheckBox * inputBlackAndWhite;
	wxSpinCtrl * inputEpochCount;

	kmeansFrame(wxFrame *frame);
	~kmeansFrame();

private:

  enum
	{
		idMenuQuit = 1000,
		idMenuAbout,
		idButtonOpenImage,
		idButtonRunAlgorithm,
		idInputTextQuantization,
		idInputEpochCount,
		ID_CHART_VIEWER_1,
		ID_CHART_VIEWER_2
	};

	void loadImages();
	wxImage * getImageByIndex(int index);
	void loadImage(wxString path);
	void imageProcess(kmeansParams params);
	kmeansPoint imageConvertToPoint(kmeansParams params);
	void OnClose(wxCloseEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpenImageDatabase(wxCommandEvent &event);
	void addSectionTitle(const wxString& label);
	kmeansPoint imageConvertToPoint(int index);
	void OnRunAlgorithm(wxCommandEvent &event);
	DECLARE_EVENT_TABLE()

};


#endif // KMEANSMAIN_H
