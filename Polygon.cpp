//Monastirli Chrysanthi AM 1716, Miltiadis Vasiliadis AM 2944
#include "Polygon.h"
#include "PolygonClipping.h"


Poly::Poly()
{
}


Poly::~Poly()
{
}

void Poly::TriangulatePolygon()
{
	Triangulate::Process(points, trianglePoints);
}

bool Intersecting(Vector2d& p1, Vector2d& p2, Vector2d& q1, Vector2d& q2) {  //kathara mathimatika, tipoi gia na doume an temnontai
	return (((q1.GetX() - p1.GetX())*(p2.GetY() - p1.GetY()) - (q1.GetY() - p1.GetY())*(p2.GetX() - p1.GetX()))
		* ((q2.GetX() - p1.GetX())*(p2.GetY() - p1.GetY()) - (q2.GetY() - p1.GetY())*(p2.GetX() - p1.GetX())) < 0)
		&&
		(((p1.GetX() - q1.GetX())*(q2.GetY() - q1.GetY()) - (p1.GetY() - q1.GetY())*(q2.GetX() - q1.GetX()))
			* ((p2.GetX() - q1.GetX())*(q2.GetY() - q1.GetY()) - (p2.GetY() - q1.GetY())*(q2.GetX() - q1.GetX())) < 0);
}

bool Poly::IsSelfIntersectingPolygon() // emeis de theloume aytotemnomena poligwna
{
	if (points.size() < 3) return false;
	const int pointsSize = points.size();
	for (unsigned int i = 0; i < pointsSize; i++)  //unsigned giati alliws xtipuse
	{
		if (i == pointsSize - 1) break;
		if (Intersecting(points[i], points[i + 1], points[pointsSize - 2], points[pointsSize - 1]))
		{
			return true;
		}
	}
	return false;
}

void Poly::Clip(int xmin, int xmax, int ymin, int ymax)
{
	PolygonClipping clipping;
	clipping.SetClippingArea(xmin, ymin, xmax, ymax);

	//trekse ton algorithmo
	Vector2dVector clipResult = clipping.ClipPolygon(points);

	//katharise ta simeia tou poligwnou
	this->points.clear();  //i .clear diagrafei ta points

	//vale gia simeia tou poligwnou ta nea simeia apo ton algorithmo
	for (int i = 0; i < clipResult.size(); i++)
	{
		this->points.push_back(Vector2d(clipResult[i].GetX(), clipResult[i].GetY()));
	}

	//katharise ta trigwna tou poligwnou
	trianglePoints.clear();

	//an iparxoun simeia meta tin apokopi
	if(points.size() >0)
	{
		//kane trigwnopoihsh ksana
		TriangulatePolygon();
	}
}
