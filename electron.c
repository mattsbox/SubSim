#include<GL/gl.h>
#include<GL/glu.h>
void drawElectron(float x,float y,float z,GLUquadricObj* quad)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glColor3f(1.0f,1.0f,0.0f);
	gluSphere(quad,0.25f,32,32);
	glPopMatrix();	
}
