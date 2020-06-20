#ifndef KMEANSPARAMS_H
#define KMEANSPARAMS_H

class kmeansParams
{
	public:

		// Width imagini
		// Daca width este 0 nu se face resize la imagini
		int width = 0;

		// Height imagini
		// Daca height este 0 nu se face resize la imagini
		int height = 0;

		// valoare pozitiva > 0 daca se vor reduce numarul de culori dintr-o imagine
		// sau 0 daca nu se aplica nici o reducere a numarului de culori
		// (Quantization)
		// Din curs:
		// 1.2 Resolution and quantization
		// Bit resolution This defines the number of possible intensity/colour values that a pixel
		// may have and relates to the quantization of the image information. For instance a binary
		// image has just two colours (black or white), a grey-scale image commonly has 256
		// different grey levels ranging from black to white whilst for a colour image it depends on
		// the colour range in use. The bit resolution is commonly quoted as the number of binary
		// bits required for storage at a given quantization level, e.g. binary is 2 bit, grey-scale is 8 bit
		// and colour (most commonly) is 24 bit. The range of values a pixel may take is often
		// referred to as the dynamic range of an image
		int quantization = 0;

		// true - daca se vor reduce culorile tuturor imaginilor la black and white,
		// false in caz contrar
		bool blackAndWhite = false;

		// Numarul de clustere folosit in algoritmul kmeans
		int kmeansK = 5;

		// Maximum epoci instruire in algoritmul kmeans
		int maxKmeansEpochCount = 1000;

		// Metoda folosita pentru a transforma pixelii unei imagini in date de input
		// (puncte in spatiu)
		enum ConvertImageToPointMethod { valoareMediePixeli };
		ConvertImageToPointMethod convertImageToPointMethod = valoareMediePixeli;

		kmeansParams() {}
		virtual ~kmeansParams() {}

};

#endif // KMEANSPARAMS_H
