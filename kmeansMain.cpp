/***************************************************************
 * Name:      kmeansMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Madalin Grigore-Enescu & Robert Zamfir ()
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "kmeansMain.h"
#include "kmeansKmeans.h"

//helper functions
enum wxbuildinfoformat
{
	short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
	wxString wxbuild(wxVERSION_STRING);

	if (format == long_f )
	{
#if defined(__WXMSW__)
		wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
		wxbuild << _T("-Mac");
#elif defined(__UNIX__)
		wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
		wxbuild << _T("-Unicode build");
#else
		wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
	}

	return wxbuild;
}

BEGIN_EVENT_TABLE(kmeansFrame, wxFrame)
	EVT_CLOSE(kmeansFrame::OnClose)
	EVT_MENU(idMenuQuit, kmeansFrame::OnQuit)
	EVT_MENU(idMenuAbout, kmeansFrame::OnAbout)
	EVT_BUTTON(idButtonOpenImage, kmeansFrame::OnOpenImageDatabase)
	EVT_BUTTON(idButtonRunAlgorithm, kmeansFrame::OnRunAlgorithm)
END_EVENT_TABLE()

kmeansFrame::kmeansFrame(wxFrame *frame)
	: wxFrame(frame, wxID_ANY, _("ML 2020 - K-MEANS"), wxDefaultPosition, wxSize(1440, 850))
{

	// LOG WINDOW
	wxFrame * logWindow = new wxFrame(this, wxID_ANY, _("ML 2020 - K-MEANS - Fereastra log-uri"), wxDefaultPosition, wxSize(1440, 850));
	wxSizer * logSizerHorizontal = new wxBoxSizer(wxHORIZONTAL);
	wxTextCtrl* logText = new wxTextCtrl(logWindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(600, 300), wxTE_MULTILINE|wxTE_READONLY);
	logSizerHorizontal->Add(logText, 1, wxEXPAND, 5);
	wxLog::SetActiveTarget(new wxLogTextCtrl(logText));
	logWindow->SetSizer(logSizerHorizontal);
	logWindow->Layout();
	logWindow->Show();

	wxInitAllImageHandlers();

	// 1440 x 900

	this->sizerHorizontal = new wxBoxSizer(wxHORIZONTAL);
	this->sizerVertical   = new wxBoxSizer(wxVERTICAL);
	this->sizerVertical->SetMinSize(440, 520);

	wxString imageButtonTitle = currentImageDatabasePath;
	if (imageButtonTitle.length() < 1)
	{
		imageButtonTitle = _("Selecteaza sursa imaginilor...");
	}
	this->openImageDatabaseButton = new wxButton(this, idButtonOpenImage, imageButtonTitle, wxDefaultPosition, wxSize(450, 40) );
	this->sizerVertical->Add(this->openImageDatabaseButton);

	this->sizerVertical->AddStretchSpacer(1);

	addSectionTitle(_("Parametrii algoritm K-means"));

	wxBoxSizer * tempBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * m_staticText1 = new wxStaticText(this, wxID_ANY, _("Quantization: "), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	m_staticText1->SetFont(wxFont(12, 70, 90, 90, false, wxEmptyString));
	tempBoxSizer1->Add(m_staticText1);
	this->inputTextQuantization = new wxSpinCtrl(this, idInputTextQuantization, _("Quantization"), wxDefaultPosition, wxSize(250, 35), wxSP_ARROW_KEYS, 0, 16000000, this->params.quantization);
	tempBoxSizer1->Add(this->inputTextQuantization);
	this->sizerVertical->Add(tempBoxSizer1);

	this->inputBlackAndWhite = new wxCheckBox(this, idInputTextQuantization, _("Conversie alb/negru"));
	this->inputBlackAndWhite->SetValue(this->params.blackAndWhite);
	this->sizerVertical->Add(this->inputBlackAndWhite);

	wxBoxSizer * tempBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * m_staticText2 = new wxStaticText(this, wxID_ANY, _("Numar epoci k-means: "), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	m_staticText2->SetFont(wxFont(12, 70, 90, 90, false, wxEmptyString));
	tempBoxSizer2->Add(m_staticText2);
	this->inputEpochCount = new wxSpinCtrl(this, idInputEpochCount, _("Numar epoci k-means"), wxDefaultPosition, wxSize(250, 35), wxSP_ARROW_KEYS, 0, 16000000, this->params.maxKmeansEpochCount);
	tempBoxSizer2->Add(this->inputEpochCount);
	this->sizerVertical->Add(tempBoxSizer2);

	this->sizerVertical->Add(0, 20, 0, 1, wxEXPAND, NULL);

	addSectionTitle(_("Distribuirea imaginilor in k=5 clase k-means:"));

	this->kmeansClustersGrid = new wxGrid(this, wxID_ANY, wxPoint(0, 0), wxSize(450, 350), 0);
	this->kmeansClustersGrid->SetDefaultColSize(60, FALSE);
	this->kmeansClustersGrid->SetDefaultRowSize(60, FALSE);
	this->kmeansClustersGrid->CreateGrid(5, 100);
	this->sizerVertical->Add(this->kmeansClustersGrid);

	this->sizerVertical->AddStretchSpacer(1);

	this->sizerVertical->Add(0, 20, 0, 1, wxEXPAND, NULL);
	this->runAlgorithmStepText = new wxStaticText(this, wxID_ANY, _("Pasul urmator este: invatare 5 persoane a cate 5 imagini"), wxDefaultPosition, wxDefaultSize, 0);
	this->runAlgorithmStepText->Wrap(400);
	this->runAlgorithmStepText->SetFont(wxFont(12, 70, 90, 90, false, wxEmptyString));
	this->sizerVertical->Add(this->runAlgorithmStepText);
	this->sizerVertical->Add(0, 20, 0, 1, wxALL|wxEXPAND, NULL);
	this->runAlgorithmButton = new wxButton(this, idButtonRunAlgorithm, _("Ruleaza pasul urmator"), wxDefaultPosition, wxSize(350, 40) );
	this->sizerVertical->Add(this->runAlgorithmButton);

	this->sizerHorizontal->Add(this->sizerVertical);

	wxBoxSizer * graphSizer  = new wxBoxSizer(wxVERTICAL);
	graphSizer->SetMinSize(1000, 520);

	wxPanel * chartViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(790, 490), wxNO_BORDER | wxHSCROLL, _("Grafice"));
	wxBoxSizer * chartViewPanelSizer  = new wxBoxSizer(wxVERTICAL);
	this->chartViewerClusters = new wxChartViewer(chartViewPanel, ID_CHART_VIEWER_1);
	chartViewPanelSizer->Add(this->chartViewerClusters);
	chartViewPanel->SetSizer(chartViewPanelSizer);

	this->chartViewerTesting  = new wxChartViewer(this, ID_CHART_VIEWER_2);

	graphSizer->Add(chartViewPanel, 0, wxEXPAND);
	graphSizer->Add(this->chartViewerTesting, 0, wxEXPAND);

	this->sizerHorizontal->Add(graphSizer);

	this->SetSizer(this->sizerHorizontal);
	this->Layout();

}


kmeansFrame::~kmeansFrame()
{
}

void kmeansFrame::OnClose(wxCloseEvent &event)
{
	Destroy();
}

void kmeansFrame::OnQuit(wxCommandEvent &event)
{
	Destroy();
}

void kmeansFrame::OnAbout(wxCommandEvent &event)
{
	wxString msg = wxbuildinfo(long_f);
	wxMessageBox(msg, _("Welcome to..."));
}

void kmeansFrame::OnOpenImageDatabase(wxCommandEvent &event)
{

	wxDirDialog * directoryDialog = new wxDirDialog(this, _("Selecteaza directorul continand imaginile"), wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (directoryDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		currentImageDatabasePath = directoryDialog->GetPath();
		this->openImageDatabaseButton->SetLabel(currentImageDatabasePath);
		this->loadImages();

	}

	// Clean up after ourselves
	directoryDialog->Destroy();

}

void kmeansFrame::addSectionTitle(const wxString& label)
{

	this->sizerVertical->Add(0, 20, 0, 1, wxEXPAND, NULL);

	wxStaticText * m_staticText1 = new wxStaticText(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(500);
	m_staticText1->SetFont(wxFont(12, 70, 90, 90, false, wxEmptyString));
	this->sizerVertical->Add(m_staticText1);

}

///////////////////////////////////////////////////////////////////////////////////////
// Incarca baza de date cu imagini
///////////////////////////////////////////////////////////////////////////////////////

void kmeansFrame::loadImages()
{

	wxLogMessage("Se incarca baza de date de imagini %s", this->currentImageDatabasePath);

	this->algorithmStep = 0;
	this->runAlgorithmStepText->SetLabel(_("Pasul urmator este: invatare 5 persoane a cate 5 imagini"));

	// Mai intai reseteaza toate celulele
	for (int allX = 0; allX < 5; allX++) {
		for (int allY = 0; allY < 100; allY++) {
			kmeansClustersGrid->SetCellRenderer(allX, allY, NULL);
		}
	}

	this->Refresh();
  this->Update();

  this->params.quantization = this->inputTextQuantization->GetValue();
	this->params.blackAndWhite = this->inputBlackAndWhite->GetValue();
	this->params.maxKmeansEpochCount = this->inputEpochCount->GetValue();

	this->points.clear();
	this->points.reserve(100);

	// Get availlable images list
	int imageIndex = 0;
	for (int i = 0; i < 5; i++)
	{

		wxDir dir;
		wxArrayString * files = new wxArrayString;
		wxString directory = this->currentImageDatabasePath + _("/") + wxString::Format(wxT("%i"), i+1);
		dir.GetAllFiles(directory,	files);
		int filesCount = files->GetCount();
		wxLogMessage("Persoana %i are %i imagini in directorul %s", i+1, filesCount, directory);
		for(int y=0; y < filesCount; y++)
		{

			// Incarca imaginea
			wxLogMessage("Se incarca imaginea %s", files->Item(i));

			if (this->loadedImages[imageIndex].LoadFile(files->Item(y), wxBITMAP_TYPE_ANY, -1))
			{
				wxLogMessage("Image succesfully loaded!");
			}
			else
			{
				wxLogMessage("Image load failed!");
				return;
			}

			// Pastreaza persoana asociata imaginii
			this->loadedImagesPersons[imageIndex] = i;

			// Proceseaza imaginea
			wxLogMessage("Se proceseaza imaginea...");
			if (this->params.width > 0 && this->params.height > 0)
			{
				if (this->loadedImages[imageIndex].GetWidth() != this->params.width || this->loadedImages[imageIndex].GetHeight() != this->params.height)
				{
					wxLogMessage("Redimensionare la dimensiunea standard...");
					this->loadedImages[imageIndex].Rescale(params.width, params.height, wxIMAGE_QUALITY_NORMAL);
				}

			}

			// Converteste imaginea in alb si negru
			if (this->params.blackAndWhite)
			{
				wxLogMessage("Se converteste imaginea in alb si negru...");
				this->loadedImages[imageIndex] = this->loadedImages[imageIndex].ConvertToGreyscale();
			}

			// Reduce culorile din imagine
			if (this->params.quantization > 0)
			{
				wxLogMessage("Se reduc culorile din imagine...");
				wxQuantize::Quantize(this->loadedImages[imageIndex], this->loadedImages[imageIndex], this->params.quantization, 0, wxQUANTIZE_FILL_DESTINATION_IMAGE);
			}

			// Creaza thumbnailul
			wxLogMessage("Se creaza thumbnailul folosit la afisare in UI");
			this->loadedImagesThumb[imageIndex] = this->loadedImages[imageIndex].Copy();
			this->loadedImagesThumb[imageIndex].Rescale(60, 60, wxIMAGE_QUALITY_NORMAL);

			imageIndex++;

		}

		delete files;

	}

}

// Returns image by index
wxImage * kmeansFrame::getImageByIndex(int index)
{
	return &loadedImages[index];
}

///////////////////////////////////////////////////////////////////////////////////////
// Ruleaza algoritmul k-means
///////////////////////////////////////////////////////////////////////////////////////

void kmeansFrame::OnRunAlgorithm(wxCommandEvent &event)
{

	switch (algorithmStep) {

		// calculeasa clasele pentru 5 persoane x 5 fotografii
		case 0:

			this->kmeans.learn(this, loadedImages, loadedImagesThumb, loadedImagesPersons, 5, this->kmeansClustersGrid, this->chartViewerClusters, this->params);

			this->algorithmStep = 1;
			this->runAlgorithmStepText->SetLabel(_("Pasul urmator este: invatare 5 persoane a cate 10 imagini"));

			break;

		// calculeasa clasele pentru 5 persoane x 10 fotografii
		case 1:

			this->kmeans.learn(this, loadedImages, loadedImagesThumb, loadedImagesPersons, 10, this->kmeansClustersGrid, this->chartViewerClusters, this->params);

			this->algorithmStep = 2;
			this->runAlgorithmStepText->SetLabel(_("Pasul urmator este: invatare 5 persoane a cate 15 imagini"));

			break;

		// calculeasa clasele pentru 5 persoane x 15 fotografii
		case 2:

			this->kmeans.learn(this, loadedImages, loadedImagesThumb, loadedImagesPersons, 15, this->kmeansClustersGrid, this->chartViewerClusters, this->params);

			this->algorithmStep = 3;
			this->runAlgorithmStepText->SetLabel(_("Pasul urmator este: invatare 5 persoane a cate 20 imagini"));

			break;

		// calculeasa clasele pentru 5 persoane x 20 fotografii
		case 3:

			this->kmeans.learn(this, loadedImages, loadedImagesThumb, loadedImagesPersons, 20, this->kmeansClustersGrid, this->chartViewerClusters, this->params);

			this->algorithmStep = 0;
			this->runAlgorithmStepText->SetLabel(_("Pasul urmator este: invatare 5 persoane a cate 5 imagini"));

			break;

	}

  this->Refresh();
  this->Update();

}
