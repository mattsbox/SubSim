//SubSym
//By Matt Craddock 2011

//List of key codes
#include<gdk/gdkkeysyms.h>

//OpenGL and GLU
#include<GL/gl.h>
#include<GL/glu.h>

//GTK+2.0
#include<gtk/gtk.h>

//GTKGLExt 1.0
#include<gtk/gtkgl.h>

//Math functions
#include<math.h>

//Include for printf
//#include<stdio.h>

//Include for arrays
//#include<stdlib.h>

//Methods for drawing protons and electrons
#include"proton.h"
#include"electron.h"
#include"constants.h"

//Used to track the number of zooms for when resizing the screen resets all of them
int z=0;

//A structure used to keep track of electrons with polar coordinates
struct particle
{
	float r,theta;
};

//The structure representing the lone electron
struct particle electron;

//The quadric object for drawing all of the particles
GLUquadricObj *quadric;

//Callback function called on the key-release-event and key-press-event from GTK
static gboolean handle_keys(GtkWidget *canvas,GdkEventKey *event,gpointer data)
{
	//Getting the key code from the key that was pressed from the event
	switch(event->keyval)
	{
		//Up, Down, Left and Right spin the atom around the origin, where it is drawn
		case GDK_Up:
			glRotatef(10,1.0,0.0,0.0);
		break;
		case GDK_Down:
			glRotatef(-10,1.0,0.0,0.0);
		break;
		case GDK_Left:
			glRotatef(-10,0.0,1.0,0.0);
		break;
		case GDK_Right:
			glRotatef(10,0.0,1.0,0.0);
		break;
		//Page Up and Page Down zoom in and out on the origin, where the atom is
		case GDK_Page_Up:
			//Projection mode sets the coordinate system to be
			//relative to the view
			glMatrixMode(GL_PROJECTION);

			//Translate the coordinate system towards the viewew
			glTranslatef(0.0f,0.0f,0.1f);

			//Increment one zoom in
			z++;

			//Return to the global coordinate system for drawing
			glMatrixMode(GL_MODELVIEW);
		break;
		case GDK_Page_Down:
			glMatrixMode(GL_PROJECTION);
			glTranslatef(0.0f,0.0f,-0.1f);
			z--;
			glMatrixMode(GL_MODELVIEW);
		break;
	}
	return TRUE;
}
static gboolean handle_mouse (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	printf("Hello");
	return TRUE;
}
//Makes all of the time-based changes to the scene
void iterate(void)
{
	//Keep the electron spinning in a perfect circle, rather than account for gravitational and electromagnetic forces
	electron.theta+=0.1f;
}
//Draws all of the particles in the scene
void gldraw(void)
{
	//Before drawing, sets up all of the values for the next frame
	iterate();	

	//Clear out everything to draw the scene anew
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	//Draw the proton, which includes quarks
	drawProton(0.0f,0.0f,0.0f,quadric);
	
	//Draw the electron at the Cartesian coordinates specified by its polar data
	drawElectron(electron.r*cos(electron.theta),0.0f,electron.r*sin(electron.theta),quadric);
}
//Callback functions that handles all of the GTK and GTKGLExt aspects of drawing the scene before deferring to OpenGL in the above method
static gboolean gtkdraw(GtkWidget *canvas,GdkEventExpose *event,gpointer user_data)
{
	//Sets up a rendering context for the widget
	GdkGLContext *glcontext = gtk_widget_get_gl_context(canvas);
	
	//Sets up the drawable part of the widget
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(canvas);
	
	//Initiates drawing on the GTKGLExt widget
	gdk_gl_drawable_gl_begin(gldrawable,glcontext);
	
	//Deferrs to the above method to handle creating the scene. This 
	//separates the two major technologies at use, GTK and OpenGL
	gldraw();
	
	//Shows the buffer that was just created
	gdk_gl_drawable_swap_buffers(gldrawable);
	
	//Ends drawing on the GTKGLExt widget
	gdk_gl_drawable_gl_end(gldrawable);
	
	//All is well
	return TRUE;
}
//Callback that sets the scene to be redrawn
static gboolean update_window(gpointer data)
{
	//Gets the widget, which is known to be the GTKGLExt widget since it is
	//the only widget
	GtkWidget *canvas = GTK_WIDGET(data);
	
	//Labels the widget's area invalid to indicates that it should be 
	//updated. &canvas->allocation retrieves that area
	gdk_window_invalidate_rect(canvas->window,&canvas->allocation,FALSE);
	
	//Generates the expose-event that triggers the above method, since the 
	//widget's area has been labeled invalid
	gdk_window_process_updates(canvas->window,FALSE);
	
	//All is well
	return TRUE;
}
//Callback for when the size of the window changes
static gboolean resize_window(GtkWidget *canvas,GdkEventConfigure *event, gpointer data)
{
	//Gets the rendering context for the GTKGlext widget
	GdkGLContext *glcontext = gtk_widget_get_gl_context(canvas);
	
	//Gets the drawable interface for the GTKGLExt widget
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(canvas);
	
	//Finds the new width and height of the GTKGLExt widget
	float w=canvas->allocation.width;
	float h=canvas->allocation.height;
	
	//Initiates drawing on the GTKGLExt widget
	gdk_gl_drawable_gl_begin(gldrawable,glcontext);
	
	//Sets the coordinate system to be relative to the viewer
	glMatrixMode(GL_PROJECTION);
	
	//Sets the position of the focus to the default
	glLoadIdentity();
	
	//Specifies the lower left corner, (0,0) and the width and height of the
	//GTKGLExt widget so that OpenGL can convert them to screen coordinates
	glViewport(0,0,w,h);
	
	//Specifies the angle of the field of view of the camera in degrees, the
	//aspect ratio and the near and far clipping planes for the scene
	gluPerspective(45,w/h,1,1000);
	
	//Shifts the coordinate system away from the viewer to give a good 
	//perspective. The original translation is -4.0f
	glTranslatef(0.0f,0.0f,(z-40)*0.1f);
	
	//Sets the coordinate system back to the global coordinates for drawing
	glMatrixMode(GL_MODELVIEW);
	
	//Properly ends the OpenGL activity
	gdk_gl_drawable_gl_end(gldrawable);
	
	//All is well
	return TRUE;
}
//Callback function for when closing the window is proposed. Returns that the intention to keep the window is negative
static gboolean window_delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	return FALSE;
}
//Callback function for when the window is closed. Ends the program, since it is the main window and the only one in existance
static void window_destroy_event(GtkWidget *widget,gpointer data)
{
	gtk_main_quit();
}
//Initiates particle states, windows and widgets before setting an animation loop for the GTKGLExt widget and entering the main loop
int main(int argc, char **argv)
{
	//The main window
	GtkWidget *window;
	
	//The GTKGLExt widget
	GtkWidget *canvas;
	
	//The configuration object to set up the GTKGLExt widget
	GdkGLConfig *glConfiguration;
	
	//Sets the initial position of the electron in polar coordinates
	electron.r=1.0f;
	electron.theta=0.5*PI;
	
	//Begins GTK
	gtk_init(&argc,&argv);
	
	//Begins GTKGLExt
	gtk_gl_init(&argc,&argv);
	
	//Creates the window object as a top level window
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	//Sets the size of the window
	gtk_window_set_default_size(GTK_WINDOW(window),400,400);
	
	//Allows the window to recieve key press and key release events
	gtk_widget_set_events(window,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_BUTTON_RELEASE_MASK);
	
	//Sets the handle_keys function to be called whenever a key is pressed 
	//or released, without sending any additional information
	g_signal_connect(window,"key-press-event",G_CALLBACK(handle_keys),NULL);
	g_signal_connect(window,"key-release-event",G_CALLBACK(handle_keys),NULL);
	g_signal_connect(window,"button-release-event",G_CALLBACK(handle_mouse),NULL);

	//Sets the proper callback functions for when closing the window is 
	//proposed and executed
	g_signal_connect(window,"delete-event",G_CALLBACK(window_delete_event),NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(window_destroy_event),NULL);
	
	//Creates the GTKGLExt widget
	canvas=gtk_drawing_area_new();
	
	//Places the GTKGLExt widget inside the main window
	gtk_container_add(GTK_CONTAINER(window),canvas);
	
	//Show the fully prepared window
	gtk_widget_show(window);
	
	//Sets the proper OpenGL settings inside the configuration object, 
	//allowing for depth testing, double buffering and RGBA coloring
	glConfiguration=gdk_gl_config_new_by_mode(GDK_GL_MODE_DEPTH|GDK_GL_MODE_DOUBLE|GDK_GL_MODE_RGBA);
	
	//Apply the settings of the configuration object to the GTKGLExt widget
	gtk_widget_set_gl_capability(canvas,glConfiguration,NULL,TRUE,GDK_GL_RGBA_TYPE);
	
	//Sets the GTKGLExt widget to call the gtkdraw function whenever it 
	//becomes aware that its area needs to be updated
	g_signal_connect(canvas,"expose-event",G_CALLBACK(gtkdraw),NULL);
	
	//Sets the resize_window function to handle any changes in the window 
	//dimensions
	g_signal_connect(canvas,"configure-event",G_CALLBACK(resize_window),NULL);
	
	//Shows all of the widgets in the window, simply the GTKGLExt widget
	gtk_widget_show_all(window);
	
	//Sets up the OpenGL quadric object for drawing the particles as spheres
	quadric=gluNewQuadric();
	
	//Allows smoothing and texturing of the spheres
	gluQuadricNormals(quadric,GLU_SMOOTH);
	gluQuadricTexture(quadric,GL_TRUE);
	
	//Tells OpenGL to check the depth of objects to keep them from simply 
	//being drawn in order
	glEnable(GL_DEPTH_TEST);
	
	//Allows for belnding to allow for the translucency of the outline
	glEnable(GL_BLEND);
	
	//Sets the blending mode to alpha to allow for translucency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	//Sets the update_window method to be called 30 times per second to 
	//draw the next animation frame
	g_timeout_add(1000/30,update_window,canvas);
	
	//Enters the main loop
	gtk_main();
	
	//All is well
	return 0;
}
