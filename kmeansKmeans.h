#ifndef KMEANSKMEANS_H
#define KMEANSKMEANS_H

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>

#include "kmeansPoint.h"
#include "kmeansParams.h"
#include "kmeansImageCellRenderer.h"

#include "wxchartdir/wxdemo/common/wxchartviewer.h"

// Implementare a algoritmului k-means
class kmeansKmeans
{

public:

	class kmeansKmeansCluster
	{

	public:

		kmeansKmeansCluster() {}
		virtual ~kmeansKmeansCluster() {}

		// Metoda adaugare punct
		void addPoint(int point)
		{

			this->points.push_back(point);

		}

		// Metoda stergere punct
		void removePoint(int point)
		{

			this->points.remove(point);

		}

		// Metoda calcul distanta minima de la punctul specificat
		// la centroidul acestei clase
		// Obs: distanța dintre 2 puncte - între P1 și M1 – se
		// determină prin distanța euclidiană

		// Varianta calcul cu index punct si vector de puncte
		double calculateMinimumDistance(int point, std::vector<kmeansPoint> points)
		{

			return sqrt(pow(points[point].x - this->centroid.x, 2.0) +
			            pow(points[point].y - this->centroid.y, 2.0) +
			            pow(points[point].z - this->centroid.z, 2.0));

		}

		// Varianta calcul cu punct dat
		double calculateMinimumDistance(kmeansPoint point)
		{

			return sqrt(pow(point.x - this->centroid.x, 2.0) +
			            pow(point.y - this->centroid.y, 2.0) +
			            pow(point.z - this->centroid.z, 2.0));

		}

		// Calculeaza centroidul
		// (calculam media artimetica a celor 3 coordonate x, y, z ale punctelor)
		void calculateCentroid(std::vector<kmeansPoint> pointsParam)
		{

			this->centroid.x = 0.0;
			this->centroid.y = 0.0;
			this->centroid.z = 0.0;
			int clusterSize = this->points.size();

			for (std::list<int>::iterator it= this->points.begin(); it != this->points.end(); ++it)
			{

				kmeansPoint point = pointsParam[*it];
				this->centroid.x += point.x;
				this->centroid.y += point.y;
				this->centroid.z += point.z;

			}

			this->centroid.x = this->centroid.x/clusterSize;
			this->centroid.y = this->centroid.y/clusterSize;
			this->centroid.z = this->centroid.z/clusterSize;

			wxLogMessage("Valoarea centroidului este: %f, %f, %f", this->centroid.x, this->centroid.y, this->centroid.z);


		}

		// Returneaza punctele
		std::list<int> getPoints()
		{

			return points;

		}

		// Returneaza centroidul
		kmeansPoint getCentroid()
		{

			return centroid;

		}

	private:

		// Punctele din aceasta clasa
		// Nu pastram decat indexul punctelor, nu punctele in sine
		// pe baza indexului putem calcula valorile lor
		std::list<int> points;

		// valoarea medie pentru aceasta clasă (se mai numeste centroid)
		kmeansPoint centroid;

	};

	// Vector continand clasele (clusters)
	std::vector<kmeansKmeansCluster> clusters;

	// Vector continand punctele
	std::vector<kmeansPoint> data;

	kmeansKmeans() {}
	virtual ~kmeansKmeans() {}

	// Ruleaza algoritmul de invatare
	void learn(wxFrame * frame,
	           wxImage loadedImages [],
	           wxImage loadedImagesThumbnails [],
	           int loadedImagesPersons [],
	           int imagesCount,
	           wxGrid * kmeansClustersGrid,
	           wxChartViewer * chartViewerClusters,
	           kmeansParams params)
	{

		// Se transforma imaginile in date de intrare
		data.clear();
		data.reserve(100);
		for (int personIterator = 0; personIterator < 5; personIterator++)
		{

			for (int imgI = 0; imgI < imagesCount; imgI++)
			{

				// Converteste imaginea in punct date intrare algoritm k-means
				int imageIndex = (personIterator * 20) + imgI;
				wxLogMessage("Converteste imaginea %i (%i) a persoanei %i in punct date intrare k-means", imgI+1, imageIndex, personIterator+1);
				kmeansPoint pointConverted = this->imageConvertToPoint(loadedImages[imageIndex], params);
				pointConverted.imageIndex = imageIndex;
				pointConverted.imagePerson= personIterator;
				wxLogMessage("Punctul rezultat din conversie este X = %f, Y = %f, Z = %f", pointConverted.x, pointConverted.y, pointConverted.z);

				// Adauga punctul la vectorul punctelor
				wxLogMessage("Adauga punctul la vectorul datelor de intrare");
				data.push_back(pointConverted);

			}

		}

		// Se retine numarul de puncte pentru a evita recontorizarea lor
		// in bucle etc si alte variabile cum ar fi numarul de clustere
		int dataSize = data.size();
		int kmeansK  = params.kmeansK;

		// Se aloca un array in care vom retine si actualiza
		// indexul clusterului unde a fost distribuit fiecare punct
		int pointsCluster[dataSize];

		// Se elibereaza orice cluster folosit in prealabil
		clusters.clear();

		//////////////////////////////////////////////////
		// Etapele de lucru:
		//////////////////////////////////////////////////

		// 1. Se vor distribui în mod aleator datele de intrare din
		// clasele considerate (C1, C2, ..., Ck amestecate) în K
		// clase.
		wxLogMessage("Se distribuie datele aleatoriu");

		// Se creaza clusterele
		// (se rezerva spatiu suficient pentru a evita realocarile repetate)
		this->clusters.reserve(kmeansK);
		kmeansKmeansCluster emptyCluster;
		for (int i = 0; i < kmeansK; i++)
		{

			this->clusters.push_back(emptyCluster);

		}

		// Se distribuie punctele
		int dI = 0;
		int cI = 0;
		while (dI < dataSize)
		{

			// Adauga punctul la clusterul curent cI
			clusters[cI].addPoint(dI);

			// Se retine clusterul la care a fost adaugat punctul
			pointsCluster[dI] = cI;

			// Avanseaza la urmatorul cluster
			cI++;
			if (cI >= kmeansK)
			{
				cI = 0;
			}

			// Avanseaza la noul punct
			dI++;

		}

		// 2. Se calculează valorile medii pentru fiecare clasă, notate
		// cu M1, M2, …, Mk (medii pentru clasele obținute la
		// pasul 1) (se mai numesc centroizi)
		for(cI = 0; cI < kmeansK; cI++)
		{
			clusters[cI].calculateCentroid(data);
		}

		// ACtualizeaza UI
		actualizeazaUI(frame, loadedImagesThumbnails, kmeansClustersGrid, chartViewerClusters, params);

		// Pauza pentru a se vedea progresul de la distribuirea aleatoriei la final
		usleep(500000);

		// 3. Pentru fiecare intrare X din cele K clase de la pasul 1 se
		// calculează distanța minimă față de valorile medii
		// (distanța folosită distanța euclidiană = suma pătratelor diferențelor pentrufiecarecomponentă).
		// În cazul în care distanța minimă se obține față de o
		// valoare medie diferită de media clasei curente atunci se
		// redistribuie intrarea X în clasa față de care
		// avem distanța minimă și se reactualizează valorile medii.
		kmeansPoint point;
		bool elementRedistributed = true;
		int epochCount = 0;

		// 4. Se repetăpasul 3 până când nu mai sunt redistribuiri de
		// date sau se ajunge la un număr maxim de epoci de
		// instruire.
		while (elementRedistributed && epochCount < params.maxKmeansEpochCount)
		{

			wxLogMessage("Se redistribuie datele pas %i", epochCount);

			// Nici un element nu a fost redistribuit ... INCA
			// daca valoarea nu se modifica dupa rularea buclei
			// variabila actioneaza ca o conditie de terminare
			// a buclei si inseamna ca toate punctele sunt distribuite
			// corespunzator
			elementRedistributed = false;

			for(dI = 0; dI < dataSize; dI++)
			{

				// Pastram old cluster
				int oldCluster         = pointsCluster[dI];

				// Acumulatori/comparatori pentru stabilirea distantei minime
				// a punctului curent
				// Se initializeaza cu valorile pentru clusterul curent in care se afla punctul
				double minDistance 		 = clusters[oldCluster].calculateMinimumDistance(dI, data);
				int minDistanceCluster = oldCluster;

				// Se calculeaza distanta minima a acestui punct
				// fata de centroidul fiecarui cluster in parte
				point = data[dI];
				for (cI = 0; cI < kmeansK; cI++)
				{

					// Calculam doar pentru clustere diferite de clusterul curent
					// pentru care s-a calculat deja distanta minima
					if (cI != oldCluster)
					{
						double currentMinDistance = clusters[cI].calculateMinimumDistance(dI, data);
						if (currentMinDistance < minDistance)
						{
							minDistanceCluster = cI;
							minDistance        = currentMinDistance;
						}
					}

				}

				// Se verifica daca distanta minima a fost gasita
				// pentru alt cluster decat clusterul curent in care se afla
				// deja punctul
				if (minDistanceCluster != oldCluster)
				{

					// Se sterge punctul din clusterul vechi
					clusters[oldCluster].removePoint(dI);

					// Se adauga punctul la noul cluster
					clusters[minDistanceCluster].addPoint(dI);

					// Se retine clusterul in care a fost mutat punctul
					pointsCluster[dI] = minDistanceCluster;

					// Se recalculeaza centroizii doar pentru clasa
					// de unde a fost mutat punctul si clasa in care a fost mutat
					clusters[oldCluster].calculateCentroid(data);
					clusters[minDistanceCluster].calculateCentroid(data);

					// Se retine faptul ca a aparut o modificare
					// pentru a nu se verifica conditia de terminare
					// while (elementRedistributed...
					elementRedistributed = true;

				}
				// else - nu se modifica nimic, se pastreaza punctul in clusterul curent

			}

			// Se incrementeaza numarul de epoci
			epochCount++;

			// ACtualizeaza UI
			actualizeazaUI(frame, loadedImagesThumbnails, kmeansClustersGrid, chartViewerClusters, params);

		}

	}

	// Algoritmul de testare
	// Returneaza clusterul imaginii de test
	int test(wxImage imageToTest, kmeansParams params) {

		// Converteste imaginea in punct date intrare algoritm k-means
		kmeansPoint pointConverted = this->imageConvertToPoint(imageToTest, params);

		// Stabileste care este clusterul cu distanta minima
		// unde apartine acest punct
		double minDistance = 0.0;
		int minDistanceCluster = -1;
		for(int cI = 0; cI < params.kmeansK; cI++)
		{

			double currentDistance = clusters[cI].calculateMinimumDistance(pointConverted);
			if (minDistanceCluster < 0 || minDistance > currentDistance) {
				minDistance = currentDistance;
				minDistanceCluster = cI;
			}

		}

		return minDistanceCluster;

	}

	// Converteste imaginea in date de intrare (punct format din 3 coordonate)
	kmeansPoint imageConvertToPoint(wxImage image, kmeansParams params)
	{

		const unsigned char * imgData = image.GetData();
		int imgPixelSize 	= image.GetWidth() * image.GetHeight();
		int imgByteSize 	= imgPixelSize * 3;
		kmeansPoint point;

		// Converteste folosind metode diferite, parametrizabile
		switch(params.convertImageToPointMethod)
		{

		// valoareMediePixeli
		default:

			int i = 0;
			int R;
			int G;
			int B;

			while (i < imgByteSize)
			{

				R = imgData[i];
				i++;
				G = imgData[i];
				i++;
				B = imgData[i];
				i++;

				point.x = point.x + (double)R;
				point.y = point.y + (double)G;
				point.z = point.z + (double)B;

			}

			break;

		}

		point.x = point.x/(double)imgPixelSize;
		point.y = point.y/(double)imgPixelSize;
		point.z = point.z/(double)imgPixelSize;

		return point;

	}

	// Actualizeaza UI-ul
	void actualizeazaUI(wxFrame * frame, wxImage loadedImages [], wxGrid * kmeansClustersGrid, wxChartViewer * chartViewerClusters, kmeansParams params)
	{

		// Mai intai reseteaza toate celulele
		for (int allX = 0; allX < 5; allX++)
		{
			for (int allY = 0; allY < 100; allY++)
			{
				kmeansClustersGrid->SetCellRenderer(allX, allY, NULL);
			}
		}


		for(int cI = 0; cI < params.kmeansK; cI++)
		{

			kmeansPoint centroid = clusters[cI].getCentroid();
			std::list<int> pointsIndexes = clusters[cI].getPoints();
			int cZ = 0;
			for (std::list<int>::iterator it = pointsIndexes.begin(); it != pointsIndexes.end(); ++it)
			{

				int pointIndex = *it;
				int imageIndex = data[pointIndex].imageIndex;
				kmeansClustersGrid->SetCellRenderer(cI, cZ, new kmeansImageCellRenderer(loadedImages[imageIndex]));

				cZ++;

			}

		}

		// Creeaza graficul 3d al clusterelor
		ThreeDScatterChart * c = new ThreeDScatterChart(790, 500);
		c->addTitle("K-means clase (clusters)            ", "timesi.ttf", 20);
		// Seteaza centrul regiunii de plot la (350, 240)
		// si seteaza width x depth x height la 360 x 360 x 270 pixels
		c->setPlotRegion(350, 240, 360, 360, 270);
		// Set the elevation and rotation angles to 15 and 30 degrees
		c->setViewAngle(15, 30);
		// Add a legend box at (600, 180)
		c->addLegend(600, 180);
		// Seteaza titlurile axelor
		c->xAxis()->setTitle("Axa-X");
		c->yAxis()->setTitle("Axa-Y");
		c->zAxis()->setTitle("Axa-Z");


		// Foloseste culori predefinite pentru grupuri pentru a fi mai vizibile
		int groupColors[5] = {0xff0000, 0x00ff00, 0x0000ff, 0xf0f000, 0x00f0f0};

		double xDataCA[5];
		double yDataCA[5];
		double zDataCA[5];

		for(int cI = 0; cI < params.kmeansK; cI++)
		{

			kmeansPoint centroid = clusters[cI].getCentroid();
			xDataCA[cI] = centroid.x;
			yDataCA[cI] = centroid.y;
			zDataCA[cI] = centroid.z;
			int groupColor = groupColors[cI];

			// Construieste datele graph-ului 3d din valoarea punctelor aflate in acest cluster
			std::list<int> pointsIndexes = clusters[cI].getPoints();
			int cZ = 0;
			int pointsCount = pointsIndexes.size();
			double xDataA[pointsCount];
			double yDataA[pointsCount];
			double zDataA[pointsCount];
			for (std::list<int>::iterator it = pointsIndexes.begin(); it != pointsIndexes.end(); ++it)
			{

				int pointIndex = *it;
				xDataA[cZ] = data[pointIndex].x;
				yDataA[cZ] = data[pointIndex].y;
				zDataA[cZ] = data[pointIndex].z;

				cZ++;

			}

			DoubleArray xData(xDataA, pointsCount);
			DoubleArray yData(yDataA, pointsCount);
			DoubleArray zData(zDataA, pointsCount);
			c->addScatterGroup(xData, yData, zData, wxString::Format(wxT("C%i"), (cI+1)), Chart::GlassSphere2Shape, 9, groupColor);

		}

		// Output the chart
		c->makeChart();
		const char *imageMap = 0;
		chartViewerClusters->setChart(c);
		chartViewerClusters->setImageMap(imageMap);

		// Layout the charts
		frame->SendSizeEvent();
		//this->Thaw();


		// Refresh UI
		kmeansClustersGrid->ForceRefresh();
		kmeansClustersGrid->Refresh();
		kmeansClustersGrid->Update();

	}


};


#endif // KMEANSKMEANS_H
