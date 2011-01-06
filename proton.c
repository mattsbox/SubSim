//SubSym
//By Matt Craddock 2011

//Imports the OpenGL functions
#include<GL/gl.h>
//Imports the GLU functions
#include<GL/glu.h>
//Imports the mathematical functions
#include<math.h>
//Draws the proton in the given Cartesian position with thr given GLU quadric
void drawProton(float x,float y,float z,GLUquadricObj* quad)
{
	//Saves the current matrix to avoid disturbing the view
	glPushMatrix();
	//Moves to a position a distance above the proton equal to the quark triangle's radius
	glTranslatef(x,y+0.25f,z);
	//Sets the color of the quark
	glColor3f(0.0f,1.0f,0.0f);
	//Draws the quark with the given radius and 32 faces in each direction
	gluSphere(quad,0.2f,32,32);
	//Moves counter-clockwise to the next corner of the quark triangle
	glTranslatef(-0.25f*cos(60),-0.25f+(0.25f*sin(60)),0.0f);
	//Sets the color of the next quark
	glColor3f(0.0f,0.0f,1.0f);
	//Draws the quark with the same characteristics
	gluSphere(quad,0.2f,32,32);
	//Moves counter-clockwise to the next corner of the quark triangle
	glTranslatef(0.5f*cos(60),0.0f,0.0f);
	//Sets the color of the third quark
	glColor3f(1.0f,0.0f,0.0f);
	//Draws the quark with the same characteristics
	gluSphere(quad,0.2f,32,32);
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
	gluSphere(quad,0.5f,32,32);
	//Returns to the initial matrix
	glPopMatrix();
}
