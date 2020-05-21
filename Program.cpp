//Chrysanthi Monastirli AM 1716, Miltiadis Vasileiadis AM 2944

//genika gia to 3D modelo empneustikame kai apo ton Jamie King

#include <iostream>
#include <GL/glut.h>
#include <vector>
#include "Polygon.h"
#include "colorDefinitions.h"
#include "menuDefinitions.h"
#include <list>
#include "Camera3D.h"
#include "Program.h"

using namespace std; //gia na min mas xtipisei lathos me onomata klp

struct window
{
	int width;
	int height;
	float aspect;		//gia tin opsi, na min exume paramorfwsi
	float nearField;	//poso konta
	float farField;		//poso makria
};
window Window;
int menu; 			//points to mainMenu
bool mPolygon = false;
bool mClipRectangle = false;
int clipStatus = -1; 		//1 otan ginetai twra to clipping, 0 otan exei ginei oloklirothike
bool mExtrude = false;
bool showTriangles = false;
float fColorR = 1.0f, fColorG = 1.0f, fColorB = 1.0f; //default mavro
float lColorR = 0.0f, lColorG = 0.0f, lColorB = 0.0f; // default lefko

//metavlites gia apokopi
int clipXmin, clipXmax, clipYmin, clipYmax, clip_enter, clip_flag;
bool upArrow, downArrow, leftArrow, rightArrow;
bool left_mouse_button = false;
bool middle_mouse_button = false;
Camera3D camera;
bool enableCamera = false;
float cameraRotationSpeed = 3.14159265358979323846264338327950288f / 180 * 0.3f; //gia tin taxitita peristrofis pou theloyme
const float cameraTranslationSpeed = 0.6;   //taxitita tou translate tis cameras,emeis to exoume sxetika xamilo
vector<Poly> Polygons;
list<Vector2d> clipArea;
Poly currentUserPolygon;
Vector2d currentUserPoint = Vector2d(0, 0);
bool keys[256];


//dimiourgei to menou
void createMenuItems()
{
	const int actionMenu = glutCreateMenu(handleMenuClickEvent);
	glutAddMenuEntry("POLYGON", POLYGON_MENU);
	glutAddMenuEntry("CLIPPING", CLIPPING_MENU);
	glutAddMenuEntry("EXTRUDE", EXTRUDE_MENU);
	glutAddMenuEntry("Exit", EXIT_MENU);

	const int lineColorMenu = glutCreateMenu(handleMenuClickEvent);
	glutAddMenuEntry("BLACK", Black_Line);
	glutAddMenuEntry("WHITE", White_Line);
	glutAddMenuEntry("RED", Red_Line);
	glutAddMenuEntry("BLUE", Blue_Line);
	glutAddMenuEntry("YELLOW", Yellow_Line);
	glutAddMenuEntry("PINK", Pink_Line);
	glutAddMenuEntry("BRONZE", Bronze_Line);
	glutAddMenuEntry("MAGENTA", Magenta_Line);
	glutAddMenuEntry("NAVY", Navy_Line);
	glutAddMenuEntry("GREEN", Green_Line);
	glutAddMenuEntry("GOLD", Gold_Line);
	glutAddMenuEntry("LIME GREEN", LimeGreen_Line);
	glutAddMenuEntry("ORANGE", Orange_Line);
	glutAddMenuEntry("FOREST GREEN", ForestGreen_Line);
	glutAddMenuEntry("BROWN", Brown_Line);
	glutAddMenuEntry("LIGHT BLUE", LightBlue_Line);

	const int fillColorMenu = glutCreateMenu(handleMenuClickEvent);
	glutAddMenuEntry("BLACK", Black_Fill);
	glutAddMenuEntry("WHITE", White_Fill);
	glutAddMenuEntry("RED", Red_Fill);
	glutAddMenuEntry("BLUE", Blue_Fill);
	glutAddMenuEntry("YELLOW", Yellow_Fill);
	glutAddMenuEntry("PINK", Pink_Fill);
	glutAddMenuEntry("BRONZE", Bronze_Fill);
	glutAddMenuEntry("MAGENTA", Magenta_Fill);
	glutAddMenuEntry("NAVY", Navy_Fill);
	glutAddMenuEntry("GREEN", Green_Fill);
	glutAddMenuEntry("GOLD", Gold_Fill);
	glutAddMenuEntry("LIME GREEN", LimeGreen_Fill);
	glutAddMenuEntry("FOREST GREEN", ForestGreen_Fill);
	glutAddMenuEntry("ORANGE", Orange_Fill);
	glutAddMenuEntry("BROWN", Brown_Fill);
	glutAddMenuEntry("LIGHT BLUE", LightBlue_Fill);

	//main menu
	menu = glutCreateMenu(handleMenuClickEvent);
	glutAddSubMenu("ACTION", actionMenu);
	glutAddSubMenu("LINE COLOR", lineColorMenu);
	glutAddSubMenu("FILL COLOR", fillColorMenu);

	//attach main menu to right click
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//zwgrafizei to poligwno pou sxediazei o xristis
void drawPolygon()
{
	//zwgrafise to poligwno pou ftiaxnei o xristis
	if (!currentUserPolygon.points.empty())
	{
		glBegin(GL_LINE_STRIP);  //ksekina alisida sindedemenwn tmimatwn grammis logw GL_LINE_STRIP
		glColor3f(currentUserPolygon.LineColorR(), currentUserPolygon.LineColorG(), currentUserPolygon.LineColorB());
		cout << "Line color:" << currentUserPolygon.LineColorR() <<" "<< currentUserPolygon.LineColorG() << " " << currentUserPolygon.LineColorB() << endl;
		for (int i = 0; i < currentUserPolygon.points.size(); i++)
		{
			glVertex3f(currentUserPolygon.points[i].GetX(), currentUserPolygon.points[i].GetY(), 0);
		}
		if (mPolygon == false)
		{
			Vector2d &p = currentUserPolygon.points.front();
			glVertex3f(p.GetX(), p.GetY(), 0);
		}
		else
		{
			glVertex3f(currentUserPoint.GetX(), currentUserPoint.GetY(), 0);
		}
		glEnd();			//oriothetoume me glBegin kai glEnd
		glFlush();			//me glFlush ginetai ektelesi twn entolwn tis GL se peperasmeno xrono
	}
}

//zwgrafizei tin perioxi apokopis pou sxediazei o xristis
void drawClipArea()
{
	//zwgrafise tin perioxi apokopis
	glBegin(GL_LINE_LOOP);		//kleisto poligwno logw GL_LINE_LOOP
	glColor3f(0, 0, 0);		//gia na balume ta "trexonta"/current xrwmata
	glVertex2i(clipXmin, clipYmin);
	glVertex2i(clipXmin, clipYmax);
	glVertex2i(clipXmax, clipYmax);
	glVertex2i(clipXmax, clipYmin);
	glEnd();
	glFlush();
}

//zwgrafizei ta poligwna pou sxediase o xristis stis 3D ekswthimena kata ton aksona z
void drawExtrude()
{
	glClearColor(1.0, 1.0, 1.0, 0.0); //kathorizei akribes times gia ta buffer tou xrwmatos
	//emeis exoume 2 buffers, den xrisimopoioume enan gia na min menei "leuko" stin othoni
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//GL_COLOR_BUFFER_BIT ipodeiknuei oti oi buffers twra mporoun na grafoun kai to GL_DEPTH_BUFFER_BIT deixnei to bathos tou buffer.
	glLoadIdentity();
	camera.RefreshCamera();
	glTranslatef(0.0f, 0.0f, -10.0f);
//multiply the current matrix by a translation matrix,paragetai mia metafrasi apo x,y kai z,douleuoume me floats
	for (int i = 0; i < Polygons.size(); i++)
	{
		Poly &poly = Polygons[i];
		Vector2dVector points = poly.points;
		//zwgrafise ta polugwna se 3 diastaseis
		for (unsigned int i = 0; i < points.size(); i++)
		{
			int next;
			if (i >= points.size() - 1) next = 0;
			else next = i + 1;
			glBegin(GL_QUADS);
			glColor3f(poly.LineColorR(), poly.LineColorG(), poly.LineColorB());
			glVertex2f(points[i].GetX(), points[i].GetY());
			//ekswthise ta poligwna kata ton aksona +z
			glVertex3f(points[i].GetX(), points[i].GetY(), poly.extrudeSize);
			glVertex3f(points[next].GetX(), points[next].GetY(), poly.extrudeSize);

			glVertex2f(points[next].GetX(), points[next].GetY());
			glEnd();
		}
		//zwgrafise tin korifi kai to katw
		Vector2dVector triangles = poly.trianglePoints;
		if (!triangles.empty())
		{
			int trianglesCount = triangles.size() / 3;
			for (int j = 0; j < trianglesCount; j++)
			{
				const Vector2d &p1 = triangles[j * 3 + 0];
				const Vector2d &p2 = triangles[j * 3 + 1];
				const Vector2d &p3 = triangles[j * 3 + 2];
				//TOP
				glBegin(GL_TRIANGLES);
				glColor3f(poly.FillColorR(), poly.FillColorG(), poly.FillColorB());
				glVertex2f(p1.GetX(), p1.GetY());
				glVertex2f(p2.GetX(), p2.GetY());
				glVertex2f(p3.GetX(), p3.GetY());
				glEnd();
				//BOTTOM
				//topothetise ton aksona +Z
				glBegin(GL_TRIANGLES);
				glColor3f(poly.FillColorR(), poly.FillColorG(), poly.FillColorB());
				glVertex3f(p1.GetX(), p1.GetY(), poly.extrudeSize);
				glVertex3f(p2.GetX(), p2.GetY(), poly.extrudeSize);
				glVertex3f(p3.GetX(), p3.GetY(), poly.extrudeSize);
				glEnd();
			}
		}
	}
}

//zwgrafizei ta poligwna pou sxediase o xristis
void drawPolygons()
{
	//an iparxoun diathesima poligwna zwgrafise ta
	if (!Polygons.empty())
	{
		for (int i = 0; i < Polygons.size(); i++)
		{
			Poly &polygon = Polygons[i];
			//sxediasi twn trigwnwn :genika i idea einai oti xreiazomai 3 korifes gia na ginei ena trigwno
			Vector2dVector triangles = polygon.trianglePoints;
			if (triangles.size() > 0)
			{
				int trianglesCount = triangles.size() / 3;
				for (int j = 0; j < trianglesCount; j++)
				{
					const Vector2d &p1 = triangles[j * 3 + 0];
					const Vector2d &p2 = triangles[j * 3 + 1];
					const Vector2d &p3 = triangles[j * 3 + 2];
					glBegin(GL_TRIANGLES);
					glColor3f(polygon.FillColorR(), polygon.FillColorG(), polygon.FillColorB());
					glVertex2f(p1.GetX(), p1.GetY());
					glVertex2f(p2.GetX(), p2.GetY());
					glVertex2f(p3.GetX(), p3.GetY());
					glEnd();
				}
			}
			//sxediasi ekswterikis gramis poligwnou
			glDisable(GL_DEPTH_TEST); //i glDisable einai sinartisi tis opengl pou kanei disable capabilities,edw exume to DEPTH_TEST pou thewritika an einai enabled kanei sigkriseis k ananewnei buffer
			Vector2dVector points = polygon.points;
			if (!points.empty())
			{
				glBegin(GL_LINE_LOOP);
				glColor3f(polygon.LineColorR(), polygon.LineColorG(), polygon.LineColorB());
				int pointsCount = points.size();
				for (int j = 0; j < pointsCount; j++)
				{
					glVertex3f(float(points[j].GetX()), float(points[j].GetY()), 0);
				}
				glEnd();
			}
		}
	}
}

//zwgrafizei ta trigwna pou prokiptoun apo ta poligwna pou sxediase o xristis
void drawTriangles()
{
	if (!Polygons.empty())
	{
		for (int i = 0; i < Polygons.size(); i++)
		{
			Poly &polygon = Polygons[i];
			Vector2dVector triangles = polygon.trianglePoints;
			if (!triangles.empty())
			{
				int trianglesCount = triangles.size() / 3;
				for (int j = 0; j < trianglesCount; j++)
				{
					const Vector2d &p1 = triangles[j * 3 + 0];
					const Vector2d &p2 = triangles[j * 3 + 1];
					const Vector2d &p3 = triangles[j * 3 + 2];
					glBegin(GL_LINE_STRIP);
					glColor3f(0.0f, 1.0f, 0.0f);
					glVertex2f(p1.GetX(), p1.GetY());
					glVertex2f(p2.GetX(), p2.GetY());
					glVertex2f(p3.GetX(), p3.GetY());
					glEnd();
				}
			}
		}
	}
}

//zwgrafizei ta antikeimena stin othoni,einai analogi tis display
void render()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (!mExtrude)
	{
		if (mPolygon)
		{
			//o xristis zwgrafizei ena poligwno
			drawPolygon();
		}

		if (mClipRectangle)
		{
			//o xristis zwgrafizei tin perioxi tis apokopis
			drawClipArea();
		}
		if (Polygons.size() > 0)
		{
			//an iparxoun diathesima poligwna zwgrafise ta
			drawPolygons();
		}
		if (showTriangles)
		{
			drawTriangles();
		}
	}
	else
	{
		//eimaste se leitourgia ekswthisis twn poligwnwn
		drawExtrude();
	}

	glutSwapBuffers();
}

//xeirizetai ta klik tou pontikiou
void handleUserClick(int button, int state, int x, int y)
{
	Vector2d point = Vector2d(x, Window.height - y);
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (mClipRectangle) clipArea.push_front(point);
			if (mPolygon)
			{
				currentUserPoint = point;
				currentUserPolygon.points.push_back(point);
				if (currentUserPolygon.IsSelfIntersectingPolygon())//an einai aytotemnomeno to poligwno ,tote ayto sbinei
				{
					currentUserPolygon.points.clear();
					return;
				}
			}
			if(mExtrude)
			{
				//move down
				left_mouse_button = true;
			}
		}
		if(state == GLUT_UP)
		{
			if(mExtrude)
			{
				left_mouse_button = false;
			}
		}

	}

	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (mPolygon)
			{
				mPolygon = false;
				if (currentUserPolygon.points.size() > 0)
				{
					//trigwnopoihse to poligwno
					currentUserPolygon.TriangulatePolygon();
					Polygons.push_back(currentUserPolygon);
				}
				createMenuItems();
				glutPostRedisplay();
			}
		/*	if (mExtrude)
			{
				//move up
				right_mouse_button = true;
			}

		}
		if (state == GLUT_UP)
		{
			if (mExtrude)
			{
				right_mouse_button = false;
			}*/
		}
	}
	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (mExtrude)
			{
				//move up
				middle_mouse_button = true;
			}
		}

		if (state == GLUT_UP)
		{
			if (mExtrude)
			{
				middle_mouse_button = false;
			}
		}
	}
}

//arxikopoihsh tu para8irou
void windowInit()
{
	Window.width = 600;
	Window.height = 500;
	Window.aspect = Window.width / Window.height;
	Window.nearField = 0.1f;
	Window.farField = 1000.0f;

	glutInitWindowSize(Window.width, Window.height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Exercise 1-1");
}

	//arxikopoihsh tis gl
void initGL()
{
	glClearColor(1, 1, 1, 1);
	//me to projection stin ousia kanume set up o,ti thelume na dume se ayto pou dimiourgisame
	glMatrixMode(GL_PROJECTION); //The projection matrix defines the properties of the camera that views the objects in the world coordinate frame.
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glViewport(0, 0, Window.width, Window.height);
	//set up orthographic
	glOrtho(0, Window.width, 0, Window.height, -0.1f, 1.0f);
	glMatrixMode(GL_MODELVIEW); //modelview matrix giati kanume emtasximatismous ston pragmatiko kosmo, px define mia fora kai meta translate,peristrofi i klimakwsi
}

//xeirizetai tin eisodo tou xristi apo to pliktrologio
void handleKeyboardInput(unsigned char key, int x, int y)
{
	keys[key] = true;
	if (key == 'T' || key == 't')
	{
		showTriangles = !showTriangles;
	}
	else if (key == ' ')
	{
		enableCamera = !enableCamera;
	}

	glutPostRedisplay(); //simatodotei to trexon "parathiro" ws aparaitito na ksanaproblithei
}


void handleKeyboardKeyUp(unsigned char key, int x, int y)
{
	keys[key] = false;
	/*
	if (key == GLUT_KEY_UP)
{
upArrow = false;
}
if (key == GLUT_KEY_DOWN)
{
downArrow = false;
}
if (key == GLUT_KEY_LEFT)
{
leftArrow = false;
}
if (key == GLUT_KEY_RIGHT)
{
rightArrow = false;
}*/
}
//velakia gia move
void handleSpecialKeyboardInput(int key,int x,int y)
{
	if(key == GLUT_KEY_UP)
	{
		upArrow = true;
	}
	if (key == GLUT_KEY_DOWN)
	{
		downArrow = true;
	}
	if (key == GLUT_KEY_LEFT)
	{
		leftArrow = true;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		rightArrow = true;
	}
}
//kalietai otan o xristis afinei to koumpi

void handleSpecialKeyboardUp(int key,int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		upArrow = false;
	}
	if (key == GLUT_KEY_DOWN)
	{
		downArrow = false;
	}
	if (key == GLUT_KEY_LEFT)
	{
		leftArrow = false;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		rightArrow = false;
	}
}




//xeirizetai tin epilogi tou menou apo ton xristi
void handleMenuClickEvent(int option)
{
	if (option == EXIT_MENU) exit(0);

	if (option == POLYGON_MENU)
	{
		mPolygon = !mPolygon;
		//thelei na dimiourgisei mia nea poligwniki grammi
		currentUserPolygon = Poly();
		currentUserPolygon.SetLineColor(lColorR, lColorG, lColorB);
		currentUserPolygon.SetFillColor(fColorR, fColorG, fColorB);
		currentUserPolygon.points.clear();
		currentUserPolygon.trianglePoints.clear();
	}
	else if (option == CLIPPING_MENU)
	{
		mClipRectangle = !mClipRectangle;
		if (clipStatus == -1 && mClipRectangle == true)
		{
			//ksekinaei to cliping
			clipStatus = 1;
			clip_enter = -1;
		}
		if (clipStatus == 1 && mClipRectangle == false)
		{
			//teleiwse cliping
			clipStatus = -1;
			//gia kathe ena poligwno trekse ton algorithmo
			for (int i = 0; i < Polygons.size(); i++)
			{
				Polygons[i].Clip(clipXmin, clipXmax, clipYmin, clipYmax);
			}
			//katharise tin perioxi apokopis
			clipArea.clear();
		}
	}
	else if (option == EXTRUDE_MENU)
	{
		//enableCamera = true;
		for (int i=0;i<Polygons.size();i++)
		{
			Polygons[i].extrudeSize = 100;
		}
		camera.Init(0.0f, 0.0f, 1.0f);
		mExtrude = true;
		reshapeView(Window.width, Window.height);
	}
	else
	{
		switch (option)
		{
		case Black_Line:
			lColorR = 0.0f; lColorG = 0.0f; lColorB = 0.0f;
			break;
		case White_Line:
			lColorR = 1.0f; lColorG = 1.0f; lColorB = 1.0f;
			break;
		case Red_Line:
			lColorR = 1.0f; lColorG = 0.0f; lColorB = 0.0f;
			break;
		case Blue_Line:
			lColorR = 0.0f; lColorG = 0.0f; lColorB = 1.0f;
			break;
		case Yellow_Line:
			lColorR = 1.0f; lColorG = 1.0f; lColorB = 0.0f;
			break;
		case Bronze_Line:
			lColorR = 0.55f; lColorG = 0.47f; lColorB = 0.14f;
			break;
		case Magenta_Line:
			lColorR = 1.0f; lColorG = 0.0f; lColorB = 1.0f;
			break;
		case Navy_Line:
			lColorR = 0.137255f; lColorG = 0.137255f; lColorB = 0.556863f;
			break;
		case Green_Line:
			lColorR = 0.0f; lColorG = 1.0f; lColorB = 0.0f;
			break;
		case Gold_Line:
			lColorR = 0.8f; lColorG = 0.498039f; lColorB = 0.196078f;
			break;
		case LimeGreen_Line:
			lColorR = 0.196078f; lColorG = 0.8f; lColorB = 0.196078f;
			break;
		case Pink_Line:
			lColorR = 0.737255f; lColorG = 0.560784f; lColorB = 0.560784f;
			break;
		case Orange_Line:
			lColorR = 1.0f; lColorG = 0.5f; lColorB = 0.0f;
			break;
		case ForestGreen_Line:
			lColorR = 0.137255f; lColorG = 0.556863f; lColorB = 0.137255f;
			break;
		case Brown_Line:
			lColorR = 0.647059f; lColorG = 0.164706f; lColorB = 0.164706f;
			break;
		case LightBlue_Line:
			lColorR = 0.74902f; lColorG = 0.847059f; lColorB = 0.847059f;
			break;
		case Black_Fill:
			fColorR = 0.0f; fColorG = 0.0f; fColorB = 0.0f;
			break;
		case White_Fill:
			fColorR = 1.0f; fColorG = 1.0f; fColorB = 1.0f;
			break;
		case Red_Fill:
			fColorR = 1.0f; fColorG = 0.0f; fColorB = 0.0f;
			break;
		case Blue_Fill:
			fColorR = 0.0f; fColorG = 0.0f; fColorB = 1.0f;
			break;
		case Yellow_Fill:
			fColorR = 1.0f; fColorG = 1.0f; fColorB = 0.0f;
			break;
		case Bronze_Fill:
			fColorR = 0.55f; fColorG = 0.47f; fColorB = 0.14f;
			break;
		case Magenta_Fill:
			fColorR = 1.0f; fColorG = 0.0f; fColorB = 1.0f;
			break;
		case Navy_Fill:
			fColorR = 0.137255f; fColorG = 0.137255f; fColorB = 0.556863f;
			break;
		case Green_Fill:
			fColorR = 0.0f; fColorG = 1.0f; fColorB = 0.0f;
			break;
		case Gold_Fill:
			fColorR = 0.8f; fColorG = 0.498039f; fColorB = 0.196078f;
			break;
		case Pink_Fill:
			fColorR = 0.737255f; fColorG = 0.560784f; fColorB = 0.560784f;
			break;
		case LimeGreen_Fill:
			fColorR = 0.196078f; fColorG = 0.8f; fColorB = 0.196078f;
			break;
		case ForestGreen_Fill:
			fColorR = 0.137255f; fColorG = 0.556863f; fColorB = 0.137255f;
			break;
		case Orange_Fill:
			fColorR = 0.309804f; fColorG = 0.184314f; fColorB = 0.309804f;
			break;
		case Brown_Fill:
			fColorR = 0.647059f; fColorG = 0.164706f; fColorB = 0.164706f;
			break;
		case LightBlue_Fill:
			fColorR = 0.74902f; fColorG = 0.847059f; fColorB = 0.847059f;
			break;
		default:
			break;
		}
	}

	glutPostRedisplay();
}

//timer pou pragmatopoiei tin kinisi tis kameras analoga me poio pliktro exei patithei
void handleCameraMotionTimer(int value)
{
	if (mExtrude && enableCamera)
	{
		if (upArrow) {
			camera.MoveCamera(cameraTranslationSpeed);
		}
		else if (downArrow) {
			//move -speed
			camera.MoveCamera(-cameraTranslationSpeed);
		}
		else if (leftArrow) {
			//strafe speed
			camera.StrafeCamera(cameraTranslationSpeed);
		}
		else if (rightArrow) {
			//strafe -speed
			camera.StrafeCamera(-cameraTranslationSpeed);
		}
		else if (middle_mouse_button) {
			//move up y
			camera.FlyCamera(cameraTranslationSpeed);
		}
		else if (left_mouse_button) {
			camera.FlyCamera(-cameraTranslationSpeed);
		}
	}
	glutTimerFunc(1, handleCameraMotionTimer, 0);
}

//xeirizetai tin kinisi tis kameras analoga me tin kinisi tou pontikiou
void mouseHandleCameraMotion(int x, int y)
{
	// This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
	// This avoids it being called recursively and hanging up the event loop
	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}
	int dx = x - Window.width / 2;
	int dy = y - Window.height / 2;

	if (dx) {
		//rotate on vertical axis
		camera.RotateCameraYaw(cameraRotationSpeed*dx);
	}

	if (dy) {
		//camera looks up/down movement
		camera.RotateCameraPitch(cameraRotationSpeed*dy);
	}

	glutWarpPointer(Window.width / 2, Window.height / 2);

	just_warped = true;
}

//xeirizetai tin kinisi tou pontikiou kai parakolouthei xmin,ymin,xmax,ymax gia ton sxediasmo tis perioxis apokopis
//otan emfanizetai oi 3D skini prowthei ta x,y simeia gia tin kinisi tis kameras

void mouseMotion(int x, int y)
{
	y = Window.height - y;
	if (clip_enter == -1 && clip_flag)
	{
		clipXmin = x;
		clipYmin = y;
		clip_flag = 0;
	}
	else
	{
		clipXmax = x;
		clipYmax = y;
	}
	//an eimaste se 3D proboli tote xeirisou to pontiki gia kinisi tis kameras
	if (mExtrude)
	{
		mouseHandleCameraMotion(x, y);
	}
	glutPostRedisplay();
}

//xeirizetai tin kinisi tou pontikiou kai ipologizei ta xmax,ymax gia ton sxediasmo tis perioxis apokopis
//an sxediazetai poligwno tote krataei to trexwn simeio tou pontikiou
//an emfanizetai i 3D skini prowthei ta x,y simeia gia tin kinisi tis kameras
void mousePassiveMotion(int x, int y)
{
	if (clip_enter == -1 && clip_flag == 0) {
		clipXmax = x;
		clipYmax = Window.height - y;
		clip_flag = 1;
		glutPostRedisplay();
	}
	if (mPolygon)
	{
		currentUserPoint = Vector2d(x, Window.height - y);
		glutPostRedisplay();
	}
	if (mExtrude && enableCamera)
	{
		mouseHandleCameraMotion(x, y);
	}
}

//epansxediazei to parathiro se periptwsi pou allaksoun oi diastaseis tou
void reshapeView(GLsizei width, GLsizei height)
{
	glViewport(0, 0, Window.width, Window.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//an den eimaste se 3D morfi tote deixnume orthografiki proboli
	if (!mExtrude)
	{
		glOrtho(0, Window.width, 0, Window.height, -0.1f, 1.0f);
	}
	else
	{
		glClearDepth(1.0f);
		glDepthFunc(GL_LEQUAL);		//i eiserxomeni timi depth einai isi i mikroteri tis apothikeumenis timis depth
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /*ipodeiknuei tin poiothta xrwmaton,tis sintetagmene kai i pio swsti h ipsilis analisis epilogi prepei na epilegetai */
		glEnable(GL_DEPTH_TEST);
		gluPerspective(45.0f, Window.aspect, 0.1f, 2000.0f);
	}
	glMatrixMode(GL_MODELVIEW);
}

//otan emfanizetai/apokriptetai to menu tote kaleitai i sinartisi
//an o xristis thelei na sxediasei ena neo poligwno tote afairoume to menu
//apo to deksi klik wste na to xrisimopoihsoume gia na termatisoume tin sxediasi tou poligwnou
void handleMenuVisibility(int status, int x, int y)
{
	if (status == 0 && mPolygon)
	{
		glutDetachMenu(GLUT_RIGHT_BUTTON);
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	windowInit();
	initGL();

	createMenuItems();
	glutKeyboardFunc(handleKeyboardInput);		//Callback gia to pliktrologio
	glutKeyboardUpFunc(handleKeyboardKeyUp);	//Callback gia to afima tou koumpiou tou pliktrologiou
	glutSpecialFunc(handleSpecialKeyboardInput);
	glutSpecialUpFunc(handleSpecialKeyboardUp);
	glutMouseFunc(handleUserClick);			//Callback gia to klik tou pontikiou
	glutMenuStatusFunc(handleMenuVisibility);	//Callback gia tin katastasi tou menu (show/hide)
	glutMotionFunc(mouseMotion);			//Callback gia tin kinisi tou pontikiou
	glutPassiveMotionFunc(mousePassiveMotion);	//Callback gia tin kinisi tou pontikiou
	glutDisplayFunc(render);			//Callback zwgrafismaton tis skinis
	glutIdleFunc(render);				//Callback zwgrafismatos tis skinis (otan i opengl den kanei kati allo)
	glutReshapeFunc(reshapeView);			//Callback gia ton epanasxediasmo tou parathirou
	glutTimerFunc(1, handleCameraMotionTimer, 0);	//Timer gia tin kinisi tis cameras
	glutMainLoop();
	return 1;
}
