//Chrysanthi Monastirli AM 1716, Miltiadis Vasiliadis AM 2944

#pragma once
#include "Triangulate.h"   //giati to arxeio einai paron mono sto trexon directory

class Poly
{
public:
	Poly();
	~Poly();

// oi FillColorR, FillColorG kai FillColorB girnane i kathemia R,G,B antistoixa gia to gemisma
	float FillColorR() const
	{
		return fColorR;
	}

	float FillColorG() const
	{
		return fColorG;
	}

	float FillColorB() const
	{
		return fColorB;
	}

//oi LineColorR(), LineColorG() kai LineColorB() girnane i kathemia R,G,B antistoixa gia grammi
	float LineColorR() const
	{
		return lColorR;
	}

	float LineColorG() const
	{
		return lColorG;
	}

	float LineColorB() const
	{
		return lColorB;
	}

	void SetLineColor(float r, float g, float b)
	{
		lColorR = r;
		lColorG = g;
		lColorB = b;
	}

	void SetFillColor(float r, float g, float b)
	{
		fColorR = r;
		fColorG = g;
		fColorB = b;
	}

	Vector2dVector points;
	Vector2dVector trianglePoints;
	void TriangulatePolygon();
	bool IsSelfIntersectingPolygon();     //einai aytotemnomeno?
	int extrudeSize;
	void Clip(int xmin,int xmax,int ymin,int ymax);  //to kaname stis dialekseis kai sximatika to clipping ston pinaka
private:
	float fColorR, fColorG,fColorB,lColorR,lColorG,lColorB;
};
