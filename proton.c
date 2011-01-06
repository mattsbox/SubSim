//SubSym
//By Matt Craddock 2011

//Imports the OpenGL functions
#include<GL/gl.h>
//Imports the GLU functions
#include<GL/glu.h>
//Imports the mathematical functions
#include<math.h>
#include"constants.h"

float quarkRadius=0.2f, protonRadius=0.5f,quarkTriangleLegLength=0.5f,quarkTriangleRadius;
void drawQuark(GLUquadricObj* quad,float r,float g,float b)
{
	//Sets the color of the quark
	glColor3f(r,g,b);
	//Draws the quark with the given radius and 32 faces in each direction
	gluSphere(quad,quarkRadius,32,32);
}
void drawStrongNuclearForce(GLUquadricObj* quad,float angle)
{
	glRotatef(90,1.0f,0.0f,0.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);
	glColor3f(1.0f,0.0f,1.0f);
	gluCylinder(quad,0.05f,0.05f,quarkTriangleLegLength,32,32);
	glRotatef(-angle,0.0f,1.0f,0.0f);
	glRotatef(-90,1.0f,0.0f,0.0f);	
}
//Draws the proton in the given Cartesian position with thr given GLU quadric
void drawProton(float x,float y,float z,GLUquadricObj* quad)
{
	quarkTriangleRadius=quarkTriangleLegLength/(2*cos(PI/6));
	//Saves the current matrix to avoid disturbing the view
	glPushMatrix();
	//Moves to a position a distance above the proton equal to the quark triangle's radius
	glTranslatef(x,y+quarkTriangleRadius,z);
	drawQuark(quad,0.0f,1.0f,0.0f);
	drawStrongNuclearForce(quad,30);
	//Moves counter-clockwise to the next corner of the quark triangle
	glTranslatef(-quarkTriangleLegLength*sin(PI/6),-quarkTriangleLegLength*cos(PI/6),0.0f);
	drawQuark(quad,0.0f,0.0f,1.0f);
	drawStrongNuclearForce(quad,150);
	//Moves counter-clockwise to the next corner of the quark triangle
	glTranslatef(quarkTriangleLegLength,0.0f,0.0f);
	drawQuark(quad,1.0f,0.0f,0.0f);
	drawStrongNuclearForce(quad,-90);
	//Returns to the initial matrix
	glPopMatrix();
	//Allows for belnding to allow for the translucency of the proton's outline
	glEnable(GL_BLEND);
	//Sets the blending mode to alpha to allow for translucency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//Saves the initial matrix before drawing again
	glPushMatrix();
	//Moves to the center of the proton
	glTranslatef(x,y,z);
	//Sets the color and transparency of the proton's outline
	glColor4f(0.0f,0.0f,1.0f,0.2f);
	//Draws the proton's outline with the given radius and 32 faces in each direction
	gluSphere(quad,protonRadius,32,32);
	//Returns to the initial matrix
	glPopMatrix();
}
