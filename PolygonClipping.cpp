//Monastirli Chrysanthi AM 1716, Miltiadis Vasiliadis AM 2944

/* stin ousia se ayto to kommati tou kwdika kanume efarmogi tou
tou algorithmou Sutherland-Hodgman  */
#include "PolygonClipping.h"
#include <vector>
#include <list>
#include <iostream>
#include "Triangulate.h"


//2: einai gnwsto ws scope resolution operator,kanoume qualification sta onomata gia na oristoun mesa stin klasi PolygonClipping
PolygonClipping::PolygonClipping()
{
}


PolygonClipping::~PolygonClipping()
{
}

//thetw ta oria tis perioxis apokopis
void PolygonClipping::SetClippingArea(int xMin, int yMin, int xMax, int yMax)
{
	xmin = xMin;
	ymin = yMin;
	xmax = xMax;
	ymax = yMax;
}

int PolygonClipping::Inside(int x, int y, int edge)
{
//exoume 4periptwseis gia to simeio an einai mesa stin perioxi apokopis i oxi,to kanam
	switch (edge) {
	case 1: if (x < xmax) return 1; else return 0; break;
	case 2: if (y > ymax) return 1; else return 0; break;
	case 3: if (x > xmin) return 1; else return 0; break;
	case 4: if (y < ymin) return 1; else return 0; break;
	default: return 0; break;
	}
}

Vector2d* PolygonClipping::Intersect(Vector2d* S, Vector2d* P, int clip_edge) {
	float m; //div by zero error earlier
	if ((P->GetX() - S->GetX()) == 0)
		m = 0;
	else
		m = (float)(P->GetY() - S->GetY()) / (P->GetX() - S->GetX());

	float c = (float)(S->GetY()) - (m*S->GetX());

	if (clip_edge == 1) { int x = xmax; int y = (m*x) + c; return (new Vector2d(x, y)); } //bug was because of m=0 thing again
	if (clip_edge == 2) { int y = ymax; int x; if (m == 0) x = P->GetX(); else x = (y - c) / m; return (new Vector2d(x, y)); }
	if (clip_edge == 3) { int x = xmin; int y = (m*x) + c; return (new Vector2d(x, y)); }
	if (clip_edge == 4) { int y = ymin; int x; if (m == 0) x = P->GetX(); else x = (y - c) / m; return (new Vector2d(x, y)); }
}

float** PolygonClipping::out_to_in(float** inner, list<Vector2d*> out) {
	inner = new float*[out.size()];
	for (int i = 0; i < out.size(); i++) {
		inner[i] = new float[2];
	}
	size = out.size();
	while (out.size() > 0) {
		Vector2d* temp = out.front();  //i .front() girna mia anafora sto prwto stoixeio tis listas
		inner[out.size() - 1][0] = temp->GetX();
		inner[out.size() - 1][1] = temp->GetY();
		out.pop_front();
	}
	out.empty(); // girizei an i lista einai adeia, diladi an size einai miden
	return inner;  //epistrefei to eswteriko
}

float** PolygonClipping::Clip(float** inva, list<Vector2d*> out, int clip_edge)
{
	/*cout << "SHPC" << endl;
	for (int i = 0; i < size; i++)
		cout << "\n" << inva[i][0] << " " << inva[i][1];
	cout << "\nxmin - " << xmin << " ymin - " << ymin;
	cout << "\nxmax - " << xmax << " ymax - " << ymax << endl;
	cout << "\nSize - " << size << endl; */
	if (size == 0) return inva;
	s = new Vector2d(inva[size - 1][0], inva[size - 1][1]);
	for (int j = 0; j < size; j++) {
		p = new Vector2d(inva[j][0], inva[j][1]);
		cout << "\n Sx - " << s->GetX() << " Sy - " << s->GetY();
		cout << "\n Py - " << p->GetX() << " Py - " << p->GetY();
		if (Inside(p->GetX(), p->GetY(), clip_edge)) // case 1 & 4
		{
			cout << "\nCASE14" << endl;
			if (Inside(s->GetX(), s->GetY(), clip_edge)) { // case 1
				out.push_front(new Vector2d(p->GetX(), p->GetY()));
				cout << "\nCASE1 - " << p->GetX() << " " << p->GetY() << endl;
			}
			else { // case 4
				Vector2d* temp = Intersect(s, p, clip_edge);
				cout << "\nCASE4 1 - " << temp->GetX() << " " << temp->GetY() << endl;
				cout << "\nCASE4 2 - " << p->GetX() << " " << p->GetY() << endl;
				out.push_front(temp);  //i .push_front() bazei ena kainourio dianisma stin lista
				out.push_front(p);
			}
		}
		else if (Inside(s->GetX(), s->GetY(), clip_edge)) { //case 2
			Vector2d* temp = Intersect(s, p, clip_edge);
			cout << "\nCASE2 - " << temp->GetX() << " " << temp->GetY() << endl;
			out.push_front(temp);
		}
		else {
			cout << "\nCASE3";
		}
		s = p;
	}
	inva = out_to_in(inva, out);
	return inva;
}

Vector2dVector PolygonClipping::ClipPolygon(Vector2dVector polygon)
{
	size = polygon.size();
	float **pts = new float*[size];

	for (int i = 0; i < size; i++)
	{
		pts[i] = new float[2];
		pts[i][0] = polygon[i].GetX();
		pts[i][1] = polygon[i].GetY();
	}
	list<Vector2d*> result;
	pts = Clip(pts, result, 1);
	pts = Clip(pts, result, 2);
	pts = Clip(pts, result, 3);
	pts = Clip(pts, result, 4);
	Vector2dVector res;
	cout << "POLYGON \n" << "--------------" << endl;
	for (int i = 0; i < size; i++)
	{
		res.push_back(Vector2d(float(pts[i][0]), float(pts[i][1]))); // to res pou einai tipou Vector2d  kalei tin .push_back() i opoia prosthetei ena kainourio stoixeio sto telos
		cout << "X: " << pts[i][0] << " Y: " << pts[i][1] << endl;
	}
	if (size == 0) return polygon;//Gia na min apokoptei ta polygona poy den einai sto plaisio apokopis
	return res;
}
