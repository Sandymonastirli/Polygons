//Monastilri Chrysanthi AM 1716, Miltiadis Vasileiadis AM 2944
#pragma once
#include "Triangulate.h"
#include <vector>//dinamikos pinakas
#include "list"
using namespace std; //xrisimopoioume namespace gia na min exoume sigkrouseis,na ginontai oi leitourgies kanonika opws theloume

class PolygonClipping
{
public:
	PolygonClipping(); //default constructor
	~PolygonClipping(); //destructor
	Vector2dVector ClipPolygon(Vector2dVector polygon);
	void SetClippingArea(int xMin, int yMin, int xMax, int yMax);

private:
	int Inside(int x, int y, int edge);		//an einai "mesa" to simeio
	Vector2d* Intersect(Vector2d* S, Vector2d* P, int edge); 	// an temnetai
	float** out_to_in(float** inner, list<Vector2d*> out);		//apo ekso pros ta mesa
	float** Clip(float** inva, list<Vector2d*> out, int clip_edge);
	int xmin, ymin, xmax, ymax, size;
	Vector2d *p, *s;
};
