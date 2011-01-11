//SubSym
//By Matt Craddock 2011

//Imports the OpenGL functions
#include<GL/gl.h>

//Imports the GLU functions for drawing spheres
#include<GL/glu.h>

//Draws the elextron in the given Cartesian position with the given OpenGL quadric object
void drawElectron(float x,float y,float z,GLUquadricObj* quad)
{
	//Saves the current matrix to avoid altering the view
	glPushMatrix();
	
	//Shifts to the proper position for drawing the electron
	glTranslatef(x,y,z);
	
	//Sets the color of the electron to yellow
	glColor3f(1.0f,1.0f,0.0f);
	
	//Draws the electron with the radius 0.25f and 32 faces in each 
	//direction
	gluSphere(quad,0.25f,32,32);
	
	//Returns to the previous matrix
	glPopMatrix();	
}
