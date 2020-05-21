//Monastirli Chrysanthi AM 1716, Miltiadis Vasiliadis AM 2944

#pragma once          //einai mia tipopoihmeni alla eurews ipostirizomeni odigia preprocessor pou exei sxediastei gia na prokalei tin enswmatwsi tou trexontos arxeiou proeleusis mono mia fora stin sillogi.ligoteros kwdikas,apofigi sigkrousewn,beltiwsi taxititas sintaksis
#include <GL/glut.h> // GLUT, includes glu.h and gl.h


void createMenuItems();
void drawPolygon();
void drawClipArea();
void drawExtrude();
void drawPolygons();
void drawTriangles();
void render();
void handleUserClick(int button, int state, int x, int y);
void windowInit();
void initGL();
void handleKeyboardInput(unsigned char key, int x, int y);
void handleKeyboardKeyUp(unsigned char key, int x, int y);
void handleMenuClickEvent(int option);
void handleCameraMotionTimer(int value);
void mouseHandleCameraMotion(int x, int y);
void mouseMotion(int x, int y);
void mousePassiveMotion(int x, int y);
void reshapeView(GLsizei width, GLsizei height);	//to GLsizei einai tipos pou iparxei stin OpenGl, mi arnitikos diadikos akeraios gia megethi
void handleMenuVisibility(int status, int x, int y);
