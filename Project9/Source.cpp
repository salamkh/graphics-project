#include <windows.h>		// Header File For Windows
#include <fstream>
#include <iostream>
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include "sound.h"
#include<texture.h>
#include <camera.h>
#include "math.h"												    // NEW: Needed For Sqrtf
#include "Model_3DS.h"	
#include "vector"
using namespace std;
bool        isClicked = false;										// NEW: Clicking The Mouse?
bool        isRClicked = false;
bool        isDragging = false;
int mouseX = 0, mouseY = 0;
INIT initialie = INIT();
Sound sound1;
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"GLAUX.LIB")


HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

int  image, image2;
// skybox
int SKYFRONT, SKYBACK, SKYLEFT, SKYRIGHT, SKYUP, SKYDOWN;
float yRotate = 0.0;
float th = 0;
Camera MyCamera;
void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SKYFRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SKYBACK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SKYLEFT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SKYRIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SKYUP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SKYDOWN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glColor3f(1, 1, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
int Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11, road;
Model_3DS tree;
Model_3DS car;
Model_3DS lightingC;
Model_3DS box;
Model_3DS cat;
Model_3DS dog;
Model_3DS horse;
//class quad {
//private:
//	float x0, y0, z0, x1, y1, z1, r, g, b;
//public:
//	void set_cordinates(GLfloat x, GLfloat xx, GLfloat y, GLfloat yy, GLfloat z, GLfloat zz)
//	{
//		x0 = x;
//		x1 = xx;
//		y0 = y;
//		y1 = yy;
//		z0 = z;
//		z1 = zz;
//	}
//	/**************************************/
//	void set_color(GLfloat rr, GLfloat gg, GLfloat bb)
//	{
//		r = rr;
//		g = gg;
//		b = bb;
//
//	}
//	quad(GLfloat x, GLfloat xx, GLfloat y, GLfloat yy, GLfloat z, GLfloat zz, GLfloat r, GLfloat g, GLfloat b)
//	{
//		set_cordinates(x, xx, y, yy, z, zz);
//		set_color(r, g, b);
//	}
//	quad()
//	{
//		set_cordinates(0, 0, 0, -.2, 0, 0);
//		set_color(1, 0, 1);
//	}
//	void draw_front()
//	{
//		glBegin(GL_QUADS);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, 0, -1);
//		glVertex3d(x0, y0, z0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y1, z0);
//		glVertex3d(x0, y1, z0);
//
//		glEnd();
//	}
//	/**************************************/
//	void board_front()
//	{
//		glBegin(GL_LINES);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, 0, -1);
//		glVertex3d(x0, y0, z0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y1, z0);
//		glVertex3d(x0, y1, z0);
//
//		glEnd();
//	}
//	/**************************************/
//	void draw_back()
//	{
//		glBegin(GL_QUADS);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, 0, 1);
//		glVertex3d(x0, y0, z1);
//		glVertex3d(x1, y0, z1);
//		glVertex3d(x1, y1, z1);
//		glVertex3d(x0, y1, z1);
//
//		glEnd();
//	}
//	/**************************************/
//	void board_back()
//	{
//		glBegin(GL_LINES);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, 0, 1);
//		glVertex3d(x0, y0, z1);
//		glVertex3d(x1, y0, z1);
//		glVertex3d(x1, y1, z1);
//		glVertex3d(x0, y1, z1);
//
//		glEnd();
//	}
//	/**************************************/
//	void draw_right()
//	{
//		glBegin(GL_QUADS);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(1, 0, 0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y0, z1);
//		glVertex3d(x1, y1, z1);
//		glVertex3d(x1, y1, z0);
//
//		glEnd();
//	}
//	/**************************************/
//	void board_right()
//	{
//		glBegin(GL_LINE_LOOP);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(1, 0, 0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y0, z1);
//		glVertex3d(x1, y1, z1);
//		glVertex3d(x1, y1, z0);
//
//		glEnd();
//	}
//	/**************************************/
//	void draw_left()
//	{
//		glBegin(GL_QUADS);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(-1, 0, 0);
//		glVertex3d(x0, y0, z0);
//		glVertex3d(x0, y0, z1);
//		glVertex3d(x0, y1, z1);
//		glVertex3d(x0, y1, z0);
//
//		glEnd();
//	}
//	/**************************************/
//	void board_left()
//	{
//		glBegin(GL_LINE_LOOP);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(-1, 0, 0);
//		glVertex3d(x0, y0, z0);
//		glVertex3d(x0, y0, z1);
//		glVertex3d(x0, y1, z1);
//		glVertex3d(x0, y1, z0);
//
//		glEnd();
//	}
//	/**************************************/
//	void draw_top()
//	{
//		glBegin(GL_QUADS);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, 1, 0);
//		glVertex3d(x0, y1, z0);
//		glVertex3d(x1, y1, z0);
//		glVertex3d(x1, y1, z1);
//		glVertex3d(x0, y1, z1);
//
//		glEnd();
//	}
//	/**************************************/
//	void board_top()
//	{
//		glBegin(GL_LINE_LOOP);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, 1, 0);
//		glVertex3d(x0, y1, z0);
//		glVertex3d(x1, y1, z0);
//		glVertex3d(x1, y1, z1);
//		glVertex3d(x0, y1, z1);
//
//		glEnd();
//	}
//	/**************************************/
//	void draw_ground()
//	{
//		glBegin(GL_QUADS);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, -1, 0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y0, z1);
//		glVertex3d(x1, y0, z1);
//
//		glEnd();
//	}
//	/**************************************/
//	void board_ground()
//	{
//		glBegin(GL_LINES);        // Draw The Cube Using quads
//
//		glColor3f(r, g, b);
//		glNormal3f(0, -1, 0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y0, z0);
//		glVertex3d(x1, y0, z1);
//		glVertex3d(x1, y0, z1);
//
//		glEnd();
//	}
//	/**************************************/
//	void texture_front(int front)
//	{
//
//		glBindTexture(GL_TEXTURE_2D, front);
//
//		glClearColor(1, 1, 1, 1);
//		glBegin(GL_QUADS);
//		glTexCoord2d(0, 0);
//		glVertex3d(x0, y0, z0);
//		glTexCoord2d(1, 0);
//		glVertex3d(x1, y0, z0);
//		glTexCoord2d(1, 1);
//		glVertex3d(x1, y1, z0);
//		glTexCoord2d(0, 1);
//
//		glVertex3d(x0, y1, z0);
//		glEnd();
//	}
//	/**************************************/
//	void texture_back(int back)
//	{
//
//		glBindTexture(GL_TEXTURE_2D, back);
//		glClearColor(1, 1, 1, 1);
//		glColor3f(1, 1, 1);
//		glBegin(GL_QUADS);
//		glTexCoord2d(0, 0);
//		glVertex3d(x0, y0, z1);
//		glTexCoord2d(1, 0);
//		glVertex3d(x1, y0, z1);
//		glTexCoord2d(1, 1);
//		glVertex3d(x1, y1, z1);
//		glTexCoord2d(0, 1);
//		glVertex3d(x0, y1, z1);
//		glEnd();
//	}
//	/**************************************/
//	void texture_left(int left)
//	{
//		glBindTexture(GL_TEXTURE_2D, left);
//		glBegin(GL_QUADS);
//		glTexCoord2d(0, 0);
//		glVertex3d(x0, y0, z1);
//		glTexCoord2d(1, 0);
//		glVertex3d(x0, y0, z0);
//		glTexCoord2d(1, 1);
//		glVertex3d(x0, y1, z0);
//		glTexCoord2d(0, 1);
//		glVertex3d(x0, y1, z1);
//		glEnd();
//
//	}
//	/**************************************/
//	void texture_right(int right)
//	{//////////////////////////////////kkk
//
//		glBindTexture(GL_TEXTURE_2D, right);
//
//		glClearColor(1, 1, 1, 1);
//		glBegin(GL_QUADS);
//		glTexCoord2d(0, 0);
//		glVertex3d(x1, y0, z1);
//		glTexCoord2d(1, 0);
//		glVertex3d(x1, y0, z0);
//		glTexCoord2d(1, 1);
//		glVertex3d(x1, y1, z0);
//		glTexCoord2d(0, 1);
//		glVertex3d(x1, y1, z1);
//		glEnd();
//	}
//	/**************************************/
//	void texture_top(int top)
//	{
//
//		glBindTexture(GL_TEXTURE_2D, top);
//
//		glClearColor(1, 1, 1, 1);
//		glBegin(GL_QUADS);
//		glTexCoord2d(0, 0);
//		glVertex3d(x0, y1, z0);
//		glTexCoord2d(1, 0);
//		glVertex3d(x1, y1, z0);
//		glTexCoord2d(1, 1);
//		glVertex3d(x1, y1, z1);
//		glTexCoord2d(0, 1);
//		glVertex3d(x0, y1, z1);
//		glEnd();
//
//	}
//	/**************************************/
//	void texture_ground(int ground)
//	{
//
//
//		glBindTexture(GL_TEXTURE_2D, ground);
//
//		glClearColor(1, 1, 1, 1);
//		glBegin(GL_QUADS);
//		glTexCoord2d(0, 0);
//		glVertex3d(x0, y0, z0);
//		glTexCoord2d(1, 0);
//		glVertex3d(x1, y0, z0);
//		glTexCoord2d(1, 1);
//		glVertex3d(x1, y0, z1);
//		glTexCoord2d(0, 1);
//		glVertex3d(x0, y0, z1);
//		glEnd();
//
//	}
//	/**************************************/
//};
//class cube
//{
//private:
//	quad q;
//public:
//	/**************************************/
//	cube(GLfloat x, GLfloat xx, GLfloat y, GLfloat yy, GLfloat z, GLfloat zz, GLfloat r, GLfloat g, GLfloat b)
//	{
//		q.set_cordinates(x, xx, y, yy, z, zz);
//		q.set_color(r, g, b);
//	}
//	cube()
//	{
//		q.set_cordinates(0, 0, -.2, -.2, 0, 0);
//		q.set_color(1, 1, 1);
//	}
//	/**************************************/
//	void set_cordinates(float x, float xx, float y, float yy, float z, float zz)
//	{
//		q.set_cordinates(x, xx, y, yy, z, zz);
//
//	}
//	/***************************************/
//	void set_color(float r, float g, float b)
//	{
//		q.set_color(r, g, b);
//	}
//
//	/**************************************/
//	void draw_cube()
//	{
//		q.draw_back();
//		q.draw_front();
//		q.draw_ground();
//		q.draw_top();
//		q.draw_left();
//		q.draw_right();
//
//	}
//	/**************************************/
//	void board_cube()
//	{
//		q.board_back();
//		q.board_front();
//		q.board_ground();
//		q.board_left();
//		q.board_right();
//		q.board_top();
//	}
//	/**************************************/
//	void texture_cube(int back, int front, int ground, int top, int left, int right)
//	{
//
//		if (back)
//			q.texture_back(back);
//		if (front)
//			q.texture_front(front);
//		if (ground)
//			q.texture_ground(ground);
//		if (top)
//			q.texture_top(top);
//		if (left)
//			q.texture_left(left);
//		if (right)
//			q.texture_right(right);
//
//	}
//	/**************************************/
//};
//cube  build;
//quad myQuad;
enum TypeOfGround{ Grass, Street };
enum TypeOfElement{ Entryablebuilding, NotEntryablebuilding, Car, Tree, LightingColumn, Box ,Cat,Dog,Horse };
class City
{
	public:
	int x;
	int z;
	bool isAvailable;
	TypeOfGround typeOfGround;
	TypeOfElement typeOfElement;
	City()
	{
		isAvailable = true;
		typeOfGround = Grass;
	}
};
float high = 1, width = 3.5,  xt = 0;
class Element
{
public:
	float x;
	float y;
	float z;
	Element(){};

	virtual void draw() = 0;
};
class Building : public Element
{
public:
	float space;
public:
	Building(){};
	Building(float s, float x0, float y0, float z0) 
	{
		x = x0;
		y = y0;
		z = z0;
		space = s;	
	}
	virtual void drawBuilding(int Image) = 0;
};
class NotEntryableBuilding : public Building
{
public:
	NotEntryableBuilding(){};
	NotEntryableBuilding(float s, float x0, float y0, float z0) :Building(s, x0, y0, z0){}
	void draw(){};
	void drawBuilding(int Image)
	{
		glEnable(GL_TEXTURE_2D);
		float s = sqrt(space);
		glTranslated(1, 1, -10);
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y + 3 * s, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y + 3 * s, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + 3 * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y + 3 * s, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y + 3 * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y + 3 * s, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + s, y, z - s);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + 3 * s, z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y + 3 * s, z - s);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + 3 * s, z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y + 3 * s, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z - s);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + 3 * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y + 3 * s, z - s);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image3);      //Draw door
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + s, y, z - (s / 3));
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z - (2 * (s / 3)));
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + (s / 2), z - (2 * (s / 3)));
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y + (s / 2), z - (s / 3));
		glEnd();
	}
};
class EntryableBuilding : public Building
{
public:
	int numberOfFloor;
	int numberOfRoom;
public:
	EntryableBuilding(){};
	EntryableBuilding(float s, float x0, float y0, float z0, int numberOfFloor, int numberOfRoom) :Building(s, x0, y0, z0)
	{
		this->numberOfFloor = numberOfFloor;
		this->numberOfRoom = numberOfRoom;
	}
	void draw(){};
	void drawBuilding(int Image)
	{
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		float s = sqrt(space);
		float b = y, temp = y, c = z - s, h = y, high = y;
		glTranslated(1, 1, -10);
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y + numberOfFloor * s, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y + numberOfFloor * s, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + numberOfFloor * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y + numberOfFloor * s, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x, y + numberOfFloor * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y + numberOfFloor * s, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + s, y, z - s);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + numberOfFloor * s, z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s, y + numberOfFloor * s, z - s);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + numberOfFloor * s, z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y + numberOfFloor * s, z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, z - s);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s, y + numberOfFloor * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x, y + numberOfFloor * s, z - s);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, Image3);      //Draw door
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s + 0.1, y, z - (s / 3));
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + s + 0.1, y, z - (2 * (s / 3)));
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s + 0.1f, y + (s / 2), z - (2 * (s / 3)));
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s + 0.1f, y + (s / 2), z - (s / 3));
		glEnd();

		float s1 = s / numberOfRoom;

		for (int i = 1; i < numberOfRoom; i++)
		{
			glBindTexture(GL_TEXTURE_2D, Image6);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, y, z - (s1*i));
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x + s1, y, z - (s1*i));
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x + s1, y + (numberOfFloor * s), z - (s1*i));
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x, y + (numberOfFloor * s), z - (s1*i));
			glEnd();
		}
		glBindTexture(GL_TEXTURE_2D, Image6);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + s1, y, z);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + s1, y, z - s);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + s1, y + numberOfFloor * s, z - s);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + s1, y + numberOfFloor * s, z);
		glEnd();
		int i;
		for (i = 1; i < numberOfFloor; i++)
		{
			glBindTexture(GL_TEXTURE_2D, Image5);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, y + (s*i), z);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x + s, y + (s*i), z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x + s, y + (s*i), z - s);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x, y + (s*i), z - s);
			glEnd();
		}
		float  yy, zz;
		yy = y;
		zz = z;
		for (int i = 1; i <= numberOfFloor; i++)
		{
			zz = z;
			for (int j = 1; j <= numberOfRoom; j++)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, Image1);      //Draw windows
				glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(x, yy + (s1 / 3), zz - (s1 / 3));
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(x, yy + (s1 / 3), zz - (2 * (s1 / 3)));
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(x, yy + (2 * (s1 / 3)), zz - (2 * (s1 / 3)));
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(x, yy + (2 * (s1 / 3)), zz - (s1 / 3));
				glEnd();
				glDisable(GL_BLEND);
				glBindTexture(GL_TEXTURE_2D, Image9);      //Draw doors
				glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(x + s1 + 0.1f, yy, zz - (s1 / 3));
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(x + s1 + 0.1f, yy, zz - (2 * (s1 / 3)));
				glTexCoord2f(.0f, 1.0f);
				glVertex3f(x + s1 + 0.1f, yy + (s1 / 2), zz - (2 * (s1 / 3)));
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(x + s1 + 0.1f, yy + (s1 / 2), zz - (s1 / 3));
				glEnd();
				zz = zz - s1;
			}
			yy = yy + s;
		}
		// draw mas3ad
		for (i = 1; i < numberOfFloor; i++){
			glBindTexture(GL_TEXTURE_2D, Image11);
			glBegin(GL_QUADS);

			glTexCoord2f(0, 1);
			glVertex3i(x + (3 * s / 2), high, z);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2) + 2, high, z);
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2) + 2, high, z - (s / 4));
			glTexCoord2f(0, 0);
			glVertex3i(x + (3 * s / 2), high, z - (s / 4));

			glEnd();

			glBindTexture(GL_TEXTURE_2D, Image11);
			glBegin(GL_QUADS);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2), high, z - (s / 4));
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2), high + s, z - (s / 4));
			glTexCoord2f(0, 0);
			glVertex3i(x + (3 * s / 2), high + s, z);
			glTexCoord2f(0, 1);
			glVertex3i(x + (3 * s / 2), high, z);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, Image11);
			glBegin(GL_QUADS);

			glTexCoord2f(0, 1);
			glVertex3i(x + (3 * s / 2), high, z - (s / 4));
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2) + 2, high, z - (s / 4));
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2) + 2, high + s, z - (s / 4));
			glTexCoord2f(0, 0);
			glVertex3i(x + (3 * s / 2), high + s, z - (s / 4));

			glEnd();


			glBindTexture(GL_TEXTURE_2D, Image11);
			glBegin(GL_QUADS);

			glTexCoord2f(0, 1);
			glVertex3i(x + (3 * s / 2) + 2, high, z);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2) + 2, high, z - (s / 4));
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2) + 2, high + s, z - (s / 4));
			glTexCoord2f(0, 0);
			glVertex3i(x + (3 * s / 2) + 2, high + s, z);

			glEnd();

			glBindTexture(GL_TEXTURE_2D, Image11);
			glBegin(GL_QUADS);

			glTexCoord2f(0, 1);
			glVertex3i(x + (3 * s / 2), high + s, z);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2) + 2, high + s, z);
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2) + 2, high + s, z - (s / 4));
			glTexCoord2f(0, 0);
			glVertex3i(x + (3 * s / 2), high + s, z - (s / 4));

			glEnd();

			glBindTexture(GL_TEXTURE_2D, Image11);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3i(x + (3 * s / 2), high, z);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2) + 2, high, z);
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2) + 2, high + s, z);
			glTexCoord2f(0, 0);
			glVertex3i(x + (3 * s / 2), high + s, z);

			glEnd();
			//end of draw mas3ad
			//draw unknown shape
			///////////////////////////////////////////////
			//الدرج الأول

			for (float j = 0; j < (s / 2); j++){
				glBindTexture(GL_TEXTURE_2D, Image5);
				glBegin(GL_QUADS);
				glTexCoord2f(0, 1);
				glVertex3d(x + (5 * s / 4), b + 0.1, c);
				glTexCoord2d(1, 1);
				glVertex3d((x + (3 * s / 2) - 0.5), b + 0.1, c);
				glTexCoord2d(1, 0);
				glVertex3d((x + (3 * s / 2) - 0.5), b + 1, c);
				glTexCoord2d(0, 0);
				glVertex3d(x + (5 * s / 4), b + 1, c);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, Image5);
				glBegin(GL_QUADS);
				glTexCoord2d(1, 1);
				glVertex3d((x + (3 * s / 2) - 0.5), b + 1, c);
				glTexCoord2d(0, 1);
				glVertex3d(x + (5 * s / 4), b + 1, c);
				glTexCoord2d(0, 0);
				glVertex3d(x + (5 * s / 4), b + 1, c - 1);
				glTexCoord2d(1, 0);
				glVertex3d((x + (3 * s / 2) - 0.5), b + 1, c - 1);

				glEnd();
				b += 1;
				c -= 1;

			}
			// ميدة
			glBindTexture(GL_TEXTURE_2D, Image5);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 1);
			glVertex3d(x + s, b, c);
			glTexCoord2d(1, 1);
			glVertex3d((x + (3 * s / 2) - 0.5), b, c);
			glTexCoord2d(1, 0);
			glVertex3d((x + (3 * s / 2) - 0.5), b, c - 3);
			glTexCoord2d(0, 0);
			glVertex3d(x + s, b, c - 3);

			glEnd();





			///////////////////////////////////////
			// الجانب حيط
			glBindTexture(GL_TEXTURE_2D, Image);
			glBegin(GL_QUADS);

			glTexCoord2f(0, 0);
			glVertex3i((x + 0.1 + (3 * s / 2)), high + s, z - (s / 4));//z - (s / 4)
			glTexCoord2f(0, 1);
			glVertex3i((x + 0.1 + (3 * s / 2)), high, z - (s / 4));//z - (s / 4)
			glTexCoord2f(1, 1);
			glVertex3i((x + 0.1 + (3 * s / 2)), high, c - 3);//c-3
			glTexCoord2f(1, 0);
			glVertex3i((x + 0.1 + (3 * s / 2)), s + high, c - 3);

			glEnd();

			/////////////////////////////////////
			//حيط الميدة
			glBindTexture(GL_TEXTURE_2D, Image);
			glBegin(GL_QUADS);

			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2), s + high, c - 3.1);
			glTexCoord2f(0, 0);
			glVertex3i(x + s, s + high, c - 3.1);
			glTexCoord2f(0, 1);
			glVertex3i(x + s, high, c - 3.1);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2), high, c - 3.1);

			glEnd();

			//////////////////////
			if (i == 0){
				//كود الأرض
				glBindTexture(GL_TEXTURE_2D, Image);
				glBegin(GL_QUADS);

				glTexCoord2f(1, 1);
				glVertex3i(x + (3 * s / 2), high, z);
				glTexCoord2f(0, 1);
				glVertex3i(x + s, high, z);
				glTexCoord2f(0, 0);
				glVertex3i(x + s, high, c - 3.1);
				glTexCoord2f(1, 0);
				glVertex3i(x + (3 * s / 2), high, c - 3.1);

				glEnd();

			}

			glBindTexture(GL_TEXTURE_2D, Image);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3i(x + s, high, z - s);
			glTexCoord2f(1, 1);
			glVertex3i(x + s, high, c - 3.1);
			glTexCoord2f(1, 0);
			glVertex3i(x + s, b, c - 3.1);
			glTexCoord2f(0, 0);
			glVertex3i(x + s, b, z - s);
			///////////////////////////////////////////////////////////
			///// الدرج التاني

			for (float k = 0; k < (s / 2); k++){
				glBindTexture(GL_TEXTURE_2D, Image5);
				glBegin(GL_QUADS);
				glTexCoord2d(0, 1);
				glVertex3d(x + s, b + 0.1, c);
				glTexCoord2d(1, 1);
				glVertex3d(x + (5 * s / 4), b + 0.1, c);
				glTexCoord2d(1, 0);
				glVertex3d(x + (5 * s / 4), b + 1, c);
				glTexCoord2d(0, 0);
				glVertex3d(x + s, b + 1, c);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, Image5);
				glBegin(GL_QUADS);
				glTexCoord2d(1, 1);
				glVertex3d(x + (5 * s / 4), b + 1, c);
				glTexCoord2d(0, 0);
				glVertex3d(x + s, b + 1, c);
				glTexCoord2d(0, 1);
				glVertex3d(x + s, b + 1, c + 1);
				glTexCoord2d(1, 1);
				glVertex3d(x + (5 * s / 4), b + 1, c + 1);

				glEnd();


				b += 1;
				c += 1;

			}

			///////////////////////////////////////////
			glBindTexture(GL_TEXTURE_2D, Image);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3i(x + s, high, z);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2), high, z);
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2), high + s, z);
			glTexCoord2f(0, 0);
			glVertex3i(x + s, high + s, z);

			glEnd();


			//////////////////////////////////
			//door
			if (i == 0){
				glBindTexture(GL_TEXTURE_2D, Image3);
				glBegin(GL_QUADS);
				glTexCoord2f(0, 1);
				glVertex3i(x + (3 * s / 2) + 0.001, high, z - (s / 2));
				glTexCoord2f(1, 1);
				glVertex3i(x + (3 * s / 2) + 0.001, high, z - ((s / 2)) - 2);
				glTexCoord2f(1, 0);
				glVertex3i(x + (3 * s / 2) + 0.001, high + 3, z - ((s / 2)) - 2);
				glTexCoord2f(0, 0);
				glVertex3i(x + (3 * s / 2) + 0.001, high + 3, z - (s / 2));

				glEnd();


			}
			/////////////////////////////////

			//سقف
			glBindTexture(GL_TEXTURE_2D, Image);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex3i(x + s, b, z);
			glTexCoord2f(1, 1);
			glVertex3i(x + (3 * s / 2), b, z);
			glTexCoord2f(1, 0);
			glVertex3i(x + (3 * s / 2), b, c);
			glTexCoord2f(0, 0);
			glVertex3i(x + s, b, c);

			//end of unlnown shape
			glEnd();

			b = s + high;
			h += s;
			temp += s;
			high += s;
		}
	}
};
class TREE : public Element
{
public:
	void draw()
	{
		tree.pos.x = x;
		tree.pos.y = y;
		tree.pos.z = z;
		tree.Draw();
	}
};
class CAR : public Element
{
public:
	void draw()
	{
		car.pos.x = x;
		car.pos.y = y;
		car.pos.z = z;
		car.scale = 0.05;
		car.Draw();
	}
};
class LIGHTINGCOLUMN : public Element
{
public:
	void draw()
	{
		lightingC.pos.x = x;
		lightingC.pos.y = y;
		lightingC.pos.z = z;
		lightingC.Draw();
	}
};
class BOX : public Element
{
public:
	void draw()
	{
		box.pos.x = x;
		box.pos.y = y;
		box.pos.z = z;
		box.Draw();
	}
};
class STREET
{
public:
	float x1, x2, y, z1, z2;


	void drawStreet()
	{
		glBindTexture(GL_TEXTURE_2D, road);

		glClearColor(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3d(x1, y, z1);
		glTexCoord2d(1, 0);
		glVertex3d(x2, y, z1);
		glTexCoord2d(1, 1);
		glVertex3d(x2, y, z2);
		glTexCoord2d(0, 1);
		glVertex3d(x1, y, z2);
		glEnd();

	}
};
class CAT : public Element
{
public:
	void draw()
	{
		cat.pos.x = x;
		cat.pos.y = y;
		cat.pos.z = z;
		cat.scale = 0.05;
		cat.Draw();
	}
};
class DOG : public Element
{
public:
	void draw()
	{
		dog.pos.x = x;
		dog.pos.y = y;
		dog.pos.z = z;
		dog.scale = 0.05;
		dog.Draw();
	}
};
class HORSE : public Element
{
public:
	void draw()
	{
		horse.pos.x = x;
		horse.pos.y = y;
		horse.pos.z = z;
		horse.scale = 0.05;
		horse.Draw();
	}
};
NotEntryableBuilding notEntryableBuilding[100];
EntryableBuilding entryableBuilding[100];
TREE tREE[100];
CAR cAR[100];
LIGHTINGCOLUMN lIGHTINGCOLUMN [100];
BOX bOX[100];
STREET sTREET[100];
CAT cAT[100];
DOG dOG[100];
HORSE hORSE[100];
City city[1500];
float xx, zz, xx2, zz2, space, length,w1=1,w2=1,w3=1;
int numberOfFLOOR, numberOfROOM, how_many, choic = 0, part = 0, x = 0;
float eyeX = 0, eyeY = 0, eyeZ = -10, angel = -1.5;
GLfloat LightPos[] = { 0.0f, 5.0f, 4.0f, 1.0f };
GLfloat LightAmb[] = { w1, w2, w3, 1.0f };
GLfloat LightDiff[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat LightSpec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat MatAmb[] = { w1, w2, w3, 1.0f };
GLfloat MatDif[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat MatSpec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat MatShn[] = { 128.0f };
GLfloat k = 0;
void mouse(int mouseX, int mouseY, bool isClicked, bool isRClicked)
{
	if (isClicked) angel -= 0.0005;;
	if (isRClicked) angel += 0.0005;;
}
void Key(bool* keys)
{
	float eyeX1 = eyeX;
	float eyeY1 = eyeY;
	float eyeZ1 = eyeZ;

	if (keys[VK_UP])
	{
		
			eyeX1 += cos(angel)*0.1;
			eyeZ1 += sin(angel)*0.1;
			eyeX = eyeX1;
			eyeZ = eyeZ1;
		
	}

	if (keys[VK_DOWN])
	{	
			eyeX1 -= cos(angel)*0.1;
			eyeZ1 -= sin(angel)*0.1;
			eyeX = eyeX1;
			eyeZ = eyeZ1;
	}

	if (keys[VK_LEFT])
		angel -= 0.0005;

	if (keys[VK_RIGHT])
		angel += 0.0005;
	if (keys['O'])
	{
			eyeY1 += 0.5;
			eyeY = eyeY1;
	}
	if (keys['L'])
	{
			eyeY1 -= 0.5;
			eyeY = eyeY1;
		
	}

}
void Key(bool* keys, float speed)
{
	if (keys['S'])
		MyCamera.RotateX(-2 * speed);
	if (keys['W'])
		MyCamera.RotateX(2 * speed);
	if (keys['D'])
		MyCamera.RotateY(-2 * speed);
	if (keys['A'])
		MyCamera.RotateY(2 * speed);
	if (keys[VK_UP])
		MyCamera.MoveForward(1 * speed);
	if (keys[VK_DOWN])
		MyCamera.MoveForward(-1 * speed);
	if (keys[VK_RIGHT])
		MyCamera.MoveRight(1 * speed);
	if (keys[VK_LEFT])
		MyCamera.MoveRight(-1 * speed);
	if (keys['X'])
		sound1.Stop();
	if (keys['P'])
		sound1.Play();
	if (keys['M'])
	{
		w1 = 1;
		w2 = 1;
		w3 = 1;
	}
	if (keys['N'])
	{
		w1 = 0;
		w2 = 0;
		w3 = 0;
	}
   
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.50f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	initialie.InitOpenAL();
	sound1 = Sound("sound.wav");
	sound1.Play();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	road = LoadTexture("road.bmp", 255);
	image = LoadTexture("back.bmp", 255);
	image2 = LoadTexture("DU icon.bmp",255);
	SKYFRONT = LoadTexture("front.bmp", 255);
	SKYBACK = LoadTexture("back.bmp", 255);
	SKYLEFT = LoadTexture("left.bmp", 255);
	SKYRIGHT = LoadTexture("right.bmp", 255);
	SKYUP = LoadTexture("up.bmp", 255);
	SKYDOWN = LoadTexture("down.bmp", 255);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	MyCamera = Camera();
	MyCamera.Position.x = 0;
	MyCamera.Position.y = 0;
	MyCamera.Position.z = 0;
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	Image1 = LoadTexture("window.bmp", 0);
	Image2 = LoadTexture("w.bmp", 255);
	Image3 = LoadTexture("door.bmp", 255);
	Image4 = LoadTexture("sky.bmp", 255);
	Image5 = LoadTexture("plain.bmp", 255);
	Image6 = LoadTexture("cinemaFloor.bmp", 255);
	Image7 = LoadTexture("building.bmp", 255);
	Image8 = LoadTexture("cinmaWall.bmp", 255);
	Image9 = LoadTexture("cinemaDoor.bmp", 255);
	Image10 = LoadTexture("wall1.bmp", 255);
	Image11 = LoadTexture("mas.bmp", 255);
	tree = Model_3DS();
	tree.Load("tree.3ds");
	tree.Materials[0].tex.Load("wood_29.bmp");
	tree.Materials[1].tex.Load("wood_29.bmp");
	tree.Materials[2].tex.Load("wood_29.bmp");
	tree.Materials[3].tex.Load("leaf.bmp");
	tree.Materials[4].tex.Load("leaf.bmp");
	car = Model_3DS();
	car.Load("car.3ds");
	car.Materials[0].tex.Load("cinemaFloor.bmp");
	car.Materials[1].tex.Load("doorLeft.bmp");
	lightingC = Model_3DS();
	lightingC.Load("StreetLamp.3ds");
	lightingC.Materials[0].tex.Load("Render3.bmp");
	lightingC.Materials[1].tex.Load("Render3.bmp");
	lightingC.Materials[2].tex.Load("Render3.bmp");
	lightingC.Materials[3].tex.Load("down.bmp");
	box = Model_3DS();
	box.Load("Crate1.3ds");
	box.Materials[0].tex.Load("crate_1.bmp");
	box.Materials[1].tex.Load("crate_1.bmp");
	cat = Model_3DS();
	cat.Load("cat.3ds");
	cat.Materials[0].tex.Load("cat.bmp");
	//cat.Materials[1].tex.Load("cat.bmp");
	dog = Model_3DS();
	dog.Load("BASSET.3ds");
	dog.Materials[0].tex.Load("BASSET.bmp");
	//dog.Materials[1].tex.Load("BASSET.bmp");
	horse = Model_3DS();
	horse.Load("Horse.3ds");
	horse.Materials[0].tex.Load("legs.bmp");
	//horse.Materials[1].tex.Load("crate_1.bmp");
	return TRUE;										// Initialization Went OK
}

int   DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity(); // Reset The Current Modelview Matrix
	Key(keys);
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + cos(angel), eyeY, eyeZ + sin(angel), 0, 1, 0);
	glEnable(GL_TEXTURE_2D);
	Draw_Skybox(-5, 0, -50, 7 * 150, 7 * 150, 7 * 200);
	MyCamera.Render();
	Key(keys, 0.2);
	mouse(mouseX, mouseY, isClicked, isRClicked);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);
	glEnable(GL_COLOR_MATERIAL);
	for (int i = 0; i < 100; i++)
	{
		if (notEntryableBuilding[i].y == -525)
		{
			notEntryableBuilding[i].drawBuilding(Image6);
		}
		if (entryableBuilding[i].y == -525)
		{
			entryableBuilding[i].drawBuilding(Image10);
		}

		if (tREE[i].y == -525)
		{
			tREE[i].draw();
		}
		if (bOX[i].y == -520)
		{
			bOX[i].draw();
		}
		if (lIGHTINGCOLUMN[i].y == -525)
		{
			lIGHTINGCOLUMN[i].draw();
		}
		if (cAT[i].y == -1000)
		{
			cAT[i].draw();
		}
		if (dOG[i].y == -520)
		{
			dOG[i].draw();
		}
		if (hORSE[i].y == -520)
		{
			hORSE[i].draw();
		}
		if (sTREET[i].y == -525)
		{
			sTREET[i].drawStreet();
		}
	}
	for (int i = 0; i < 100; i++)
	{
		if (cAR[i].y == -523)
		{
			glTranslated(0, 0, xt);
			cAR[i].draw();
			xt += 0.005;
		}
	}
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "GL template", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	static PAINTSTRUCT ps;
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
													if (!HIWORD(wParam))					// Check Minimization State
													{
														active = TRUE;						// Program Is Active
													}
													else
													{
														active = FALSE;						// Program Is No Longer Active
													}

													return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
													switch (wParam)							// Check System Calls
													{
													case SC_SCREENSAVE:					// Screensaver Trying To Start?
													case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
														return 0;							// Prevent From Happening
													}
													break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
													PostQuitMessage(0);						// Send A Quit Message
													return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
													keys[wParam] = TRUE;					// If So, Mark It As TRUE
													return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
													keys[wParam] = FALSE;					// If So, Mark It As FALSE
													return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
													ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
													return 0;								// Jump Back
	}
	case WM_MOUSEMOVE:
	{
						 mouseX = (int)LOWORD(lParam);
						 mouseY = (int)HIWORD(lParam);
						 isClicked = (LOWORD(wParam) & MK_LBUTTON) ? true : false;
						 isRClicked = (LOWORD(wParam) & MK_RBUTTON) ? true : false;
						 break;
	}
	case WM_LBUTTONUP:
		isClicked = false; break;
	case WM_RBUTTONUP:
		isRClicked = false; break;
	case WM_LBUTTONDOWN:
		isClicked = true; break;
	case WM_RBUTTONDOWN:
		isRClicked = true; break;
	case WM_PAINT:
		DrawGLScene();
		BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		return 0;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE    hInstance,HINSTANCE	hPrevInstance,LPSTR	lpCmdLine,int nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL template", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("OpenGL template", 640, 480, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

int main(HINSTANCE hinstance, HINSTANCE hPrevlnstance, LPSTR iPCmdLine, int nCmdShow)
{	
		for (int i = -750; i <= 750; i++)
		{
			for (int j = -750; j <= 750; j++)
			{
				for (int k = 0; k < 1500; k++)
				{
				city[k].x = i;
				city[k].z = j;
			    }
		   }
	   }
	while (choic != 11)
	{
		cout << "Enter 1 if you want EntryableBuilding and how many you want of it\n";
		cout << "Enter 2 if you want NotEntryableBuilding and how many you want of it\n";
		cout << "Enter 3 if you want car and how many you want of it\n";
		cout << "Enter 4 if you want tree and how many you want of it\n";
		cout << "Enter 5 if you want lightingculumn and how many you want of it\n";
		cout << "Enter 6 if you want box and how many you want of it\n";
		cout << "Enter 7 if you want cat and how many you want of it\n";
		cout << "Enter 8 if you want dog and how many you want of it\n";
		cout << "Enter 9 if you want horse and how many you want of it\n";
		cout << "Enter 10 if you want street and how many you want of it\n";
		cout << "Enter 11 if you dont want any thing \n";
		cin >> choic;
		cin >> how_many;
		switch (choic)
		{
		case 1:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter space of builing smaller than 500 \n";
				cin >> space;
				cout << " Enter x of builing \n";
				cin >> xx;
				cout << " Enter z of builing \n";
				cin >> zz;
				cout << " Enter number of floor in builing smaller than 10\n";
				cin >> numberOfFLOOR;
				cout << " Enter number of room in floor smaller than 6 \n";
				cin >> numberOfROOM;
		
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= sqrt(space); i++)
					{
						for (int j = 0; j <= sqrt(space); j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable==false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= sqrt(space); i++)
						{
							for (int j = 0; j <= sqrt(space); j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Entryablebuilding;
							}
						}
					}
					entryableBuilding[h].space = space;
					entryableBuilding[h].x = xx;
					entryableBuilding[h].y = -525;
					entryableBuilding[h].z = zz;
					entryableBuilding[h].numberOfFloor = numberOfFLOOR;
					entryableBuilding[h].numberOfRoom = numberOfROOM;
					
				}	
			}
			break;
		case 2:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter space of builing smaller than 500\n";
				cin >> space;
				cout << " Enter x of builing smaller than 10\n";
				cin >> xx;
				cout << " Enter z of builing smaller than 6\n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= sqrt(space); i++)
					{
						for (int j = 0; j <= sqrt(space); j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= sqrt(space); i++)
						{
							for (int j = 0; j <= sqrt(space); j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = NotEntryablebuilding;
							}
						}
					}
					notEntryableBuilding[h].space = space;
					notEntryableBuilding[h].x = xx;
					notEntryableBuilding[h].y = -525;
					notEntryableBuilding[h].z = zz;
					
				}
			}
			break;
		case 3:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of car \n";
				cin >> xx;
				cout << " Enter z of car \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 10; i++)
					{
						for (int j = 0; j <= 10; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) /*|| city[k].typeOfGround == Grass*/)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 10; i++)
						{
							for (int j = 0; j <= 10; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Car;
							}
						}
					}
					cAR[h].x = xx;
					cAR[h].y = -523;
					cAR[h].z = zz;
					
				}
			}
			break;
		case 4:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of tree \n";
				cin >> xx;
				cout << " Enter z of tree \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 3; i++)
						{
							for (int j = 0; j <= 3; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Tree;
							}
						}
					}
					tREE[h].x = xx;
					tREE[h].y = -525;
					tREE[h].z = zz;
					
				}
			}
			break;
		case 5:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of lighting column \n";
				cin >> xx;
				cout << " Enter z of lighting column \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 3; i++)
						{
							for (int j = 0; j <= 3; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = LightingColumn;
							}
						}
					}
					lIGHTINGCOLUMN[h].x = xx;
					lIGHTINGCOLUMN[h].y = -525;
					lIGHTINGCOLUMN[h].z = zz;
					
				}
			}
			break;
		case 6:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of box \n";
				cin >> xx;
				cout << " Enter z of box \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 3; i++)
						{
							for (int j = 0; j <= 3; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Box;
							}
						}
					}
					bOX[h].x = xx;
					bOX[h].y = -520;
					bOX[h].z = zz;
					
				}
			}
			break;
		case 7:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of cat \n";
				cin >> xx;
				cout << " Enter z of cat \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 3; i++)
						{
							for (int j = 0; j <= 3; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Cat;
							}
						}
					}
					cAT[h].x = xx;
					cAT[h].y = -1000;
					cAT[h].z = zz;

				}
			}
			break;
		case 8:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of dog \n";
				cin >> xx;
				cout << " Enter z of dog \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 3; i++)
						{
							for (int j = 0; j <= 3; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Dog;
							}
						}
					}
					dOG[h].x = xx;
					dOG[h].y = -520;
					dOG[h].z = zz;

				}
			}
			break;
		case 9:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x of horse \n";
				cin >> xx;
				cout << " Enter z of horse \n";
				cin >> zz;
				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= 5; i++)
					{
						for (int j = 0; j <= 5; j++)
						if ((city[k].x == xx + i &&city[k].z == zz - j && city[k].isAvailable == false) || city[k].typeOfGround == Street)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= 5; i++)
						{
							for (int j = 0; j <= 5; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].isAvailable = false;
								city[i].typeOfElement = Horse;
							}
						}
					}
					hORSE[h].x = xx;
					hORSE[h].y = -520;
					hORSE[h].z = zz;
				}
			}
			break;
		case 10:
			for (int h = 0; h < how_many; h++)
			{
				bool b = false;
				cout << " Enter x1 of street \n";
				cin >> xx;
				cout << " Enter z1 of street \n";
				cin >> zz;
				cout << " Enter x2 of street \n";
				cin >> xx2;
				cout << " Enter z2 of street \n";
				cin >> zz2;

				for (int k = 0; k < 1500; k++)
				{
					for (int i = 0; i <= xx2-xx; i++)
					{
						for (int j = 0; j <= zz2-zz; j++)
						if (city[k].typeOfElement == Entryablebuilding || city[k].typeOfElement == NotEntryablebuilding || city[k].typeOfElement == Tree || city[k].typeOfElement == Box || city[k].typeOfElement == LightingColumn)
						{
							cout << "This position is not available \n";
							b = true;
							break;
						}
						if (b == true)
							break;
					}
					if (b == true)
						break;
				}
				if (b == false){
					for (int k = 0; k < 1500; k++)
					{
						for (int i = 0; i <= xx2 - xx; i++)
						{
							for (int j = 0; j <= zz2 - zz; j++)
							if (city[k].x == xx + i &&city[k].z == zz - j)
							{
								city[i].typeOfGround = Street;
							}
						}
					}
					sTREET[h].x1 = xx;
					sTREET[h].x2 = xx2;
					sTREET[h].z1 = zz;
					sTREET[h].z2 = zz2;
					sTREET[h].y = -525;
				}
			}
			break;
		case 11:
			return WinMain(hinstance, hPrevlnstance, iPCmdLine, nCmdShow);
		default:
			break;
		}
	}
	return WinMain(hinstance, hPrevlnstance, iPCmdLine, nCmdShow);
}
