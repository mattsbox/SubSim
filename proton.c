//SubSym
//By Matt Craddock 2011

//Imports the OpenGL functions
#include<GL/gl.h>

//Imports the GLU functions
#include<GL/glu.h>

//Imports the mathematical functions
#include<math.h>
#include"constants.h"

//Sets the constant radius of quarks and protons as well as the leg length of
//the quark triangle. Since the radius depends on the leg length, it is set
//in the drawProton method
float quarkRadius=0.2f, protonRadius=0.5f,quarkTriangleLegLength=0.5f,quarkTriangleRadius;

//Draws an individual quark at whatever position is currently being drawn
//with GLU quad and RGB color specified
void drawQuark(GLUquadricObj* quad,float r,float g,float b)
{
	//Sets the color of the quark
	glColor3f(r,g,b);
	//Draws the quark with the given radius and 32 faces in each direction
	gluSphere(quad,quarkRadius,32,32);
}
//Draws the strong nuclear force as a cylinder at whatever position is 
//currently being drawn, tilted in the xy plane at the given angle from
//vertically downward
void drawStrongNuclearForce(GLUquadricObj* quad,float angle)
{
	//Saves the current matrix so the rotations can be quickly undone
	glPushMatrix();
	
	//Rotates the coordinate system around a vector in the x axis so that
	//the cylinder will be in the xy plane
	glRotatef(90,1.0f,0.0f,0.0f);
	
	//Rotates the coordinate system by the given angle to set the direction
	//of the cylinder
	glRotatef(angle,0.0f,1.0f,0.0f);
	
	//Sets the color of the cylinder
	glColor3f(1.0f,0.0f,1.0f);
	
	//Draws the cylinder with the given radius on the top and bottom, the
	//given length and the given number of faces in two directions
	gluCylinder(quad,0.05f,0.05f,quarkTriangleLegLength,32,32);
	
	//Returns to the matrix being used at the beginning of this function
	glPopMatrix();
}
//Draws the proton in the given Cartesian position with thr given GLU quadric
void drawProton(float x,float y,float z,GLUquadricObj* quad)
{
	//Calculates the radius of the equilateral triangle formed by the
	//quarks based on the length of one side
	quarkTriangleRadius=quarkTriangleLegLength/(2*cos(PI/6));
	
	//Saves the current matrix to avoid disturbing the view
	glPushMatrix();
	
	//Moves to the position of the top quark
	glTranslatef(x,y+quarkTriangleRadius,z);
	
	//Draws the top quark with the given color
	drawQuark(quad,0.0f,1.0f,0.0f);
	
	//Draws the strong nuclear force going out from the bottom right
	drawStrongNuclearForce(quad,30);
	
	//Moves counter-clockwise to the next corner of the quark triangle
	glTranslatef(-quarkTriangleLegLength*sin(PI/6),-quarkTriangleLegLength*cos(PI/6),0.0f);
	
	//Draws the bottom-left quark with the given color
	drawQuark(quad,0.0f,0.0f,1.0f);
	
	//Draws the strong nuclear force going up and to the right
	drawStrongNuclearForce(quad,150);
	
	//Moves counter-clockwise to the next corner of the quark triangle
	glTranslatef(quarkTriangleLegLength,0.0f,0.0f);
	
	//Draws the bottom-right quark with the given color
	drawQuark(quad,1.0f,0.0f,0.0f);
	
	//Draws the strong nuclear force going across the bottom
	drawStrongNuclearForce(quad,-90);
	
	//Returns to the initial matrix
	glPopMatrix();	
	
	//Saves the initial matrix before drawing again
	glPushMatrix();
	
	//Moves to the center of the proton
	glTranslatef(x,y,z);
	
	//Sets the color and transparency of the proton's outline
	glColor4f(0.0f,0.0f,1.0f,0.2f);
	
	//Draws the proton's outline with the given radius and 32 faces in each
	//direction
	gluSphere(quad,protonRadius,32,32);
	
	//Returns to the initial matrix
	glPopMatrix();
}
