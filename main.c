#include<gdk/gdkkeysyms.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<gtk/gtk.h>
#include<gtk/gtkgl.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include"proton.h"
#include"electron.h"
const float PI=3.1415926535;
struct particle
{
	float r,theta;
};
struct particle electron;
GLUquadricObj *quadric;
static gboolean handle_keys(GtkWidget *canvas,GdkEventKey *event,gpointer data)
{
	guint key=event->keyval;
	switch(key)
	{
		case GDK_KEY_Up:
			glRotatef(10,1.0,0.0,0.0);
		break;
		case GDK_KEY_Down:
			glRotatef(-10,1.0,0.0,0.0);
		break;
		case GDK_KEY_Left:
			glRotatef(-10,0.0,1.0,0.0);
		break;
		case GDK_KEY_Right:
			glRotatef(10,0.0,1.0,0.0);
		break;
	}
	return TRUE;
}
void iterate(void)
{
	electron.theta+=0.1f;
}
void gldraw(void)
{
	iterate();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	drawProton(0.0f,0.0f,0.0f,quadric);
	drawElectron(electron.r*cos(electron.theta),0.0f,electron.r*sin(electron.theta),quadric);
}
static gboolean gtkdraw(GtkWidget *canvas,GdkEventExpose *event,gpointer user_data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context(canvas);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(canvas);
	gdk_gl_drawable_gl_begin(gldrawable,glcontext);
	gldraw();	
	gdk_gl_drawable_swap_buffers(gldrawable);
	gdk_gl_drawable_gl_end(gldrawable);
	return TRUE;
}
static gboolean update_window(gpointer data)
{
	GtkWidget *canvas = GTK_WIDGET(data);
	gdk_window_invalidate_rect(canvas->window,&canvas->allocation,FALSE);
	gdk_window_process_updates(canvas->window,FALSE);
	return TRUE;
}
static gboolean resize_window(GtkWidget *canvas,GdkEventConfigure *event, gpointer data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context(canvas);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(canvas);
	float w=canvas->allocation.width;
	float h=canvas->allocation.height;
	gdk_gl_drawable_gl_begin(gldrawable,glcontext);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,w,h);
	gluPerspective(45,w/h,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0,0.0,0.0,-1.0,0.0,1.0,0.0);
	gdk_gl_drawable_gl_end(gldrawable);
	return TRUE;
}
static gboolean window_delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	return FALSE;
}
static void window_destroy_event(GtkWidget *widget,gpointer data)
{
	gtk_main_quit();
}
int main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *canvas;
	GdkGLConfig *glConfiguration;
	electron.r=1.0f;
	electron.theta=0.5*PI;
	gtk_init(&argc,&argv);
	gtk_gl_init(&argc,&argv);
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),400,400);
	gtk_widget_set_events(window,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
	g_signal_connect(window,"key-press-event",G_CALLBACK(handle_keys),NULL);
	g_signal_connect(window,"key-release-event",G_CALLBACK(handle_keys),NULL);
	g_signal_connect(window,"delete-event",G_CALLBACK(window_delete_event),NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(window_destroy_event),NULL);
	canvas=gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window),canvas);
	gtk_widget_show(window);
	glConfiguration=gdk_gl_config_new_by_mode(GDK_GL_MODE_DEPTH|GDK_GL_MODE_DOUBLE|GDK_GL_MODE_RGBA);
	gtk_widget_set_gl_capability(canvas,glConfiguration,NULL,TRUE,GDK_GL_RGBA_TYPE);
	g_signal_connect(canvas,"expose-event",G_CALLBACK(gtkdraw),NULL);
	g_signal_connect(canvas,"configure-event",G_CALLBACK(resize_window),NULL);
	//glEnable(GL_DEPTH_TEST);
	gtk_widget_show_all(window);
	quadric=gluNewQuadric();
	gluQuadricNormals(quadric,GLU_SMOOTH);
	gluQuadricTexture(quadric,GL_TRUE);
	g_timeout_add(1000/30,update_window,canvas);
	gtk_main();
	return 0;
}
