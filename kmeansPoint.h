#ifndef KMEANSPOINT_H
#define KMEANSPOINT_H

class kmeansPoint
{
	public:

		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		// Aceste informatii sunt necesare doar pentru partea
		// de UI (afisare imagini in clustere)
		int imageIndex = 0;
		int imagePerson = 0;

		kmeansPoint() {}
		virtual ~kmeansPoint() {}

};

#endif // KMEANSPOINT_H
