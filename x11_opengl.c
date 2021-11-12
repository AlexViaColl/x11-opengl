#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glx.h>

#define WIDTH  	800
#define HEIGHT	600

int main() {
    Display *display = XOpenDisplay(NULL);

	XVisualInfo *visual = glXChooseVisual(display, 0, (int[]){
		GLX_RGBA,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER,
		None
	});
	if (!visual) {
		fprintf(stderr, "Unable to choose visual\n");
		exit(1);
	}
	printf("Visual ID: %x\n", visual->visualid);

	GLXContext gl_context = glXCreateContext(display, visual, 0, True);
	if (!gl_context) {
		fprintf(stderr, "Unable to create GL context\n");
		exit(1);
	}
    
    Window window = XCreateSimpleWindow(
        display,
        XDefaultRootWindow(display),    // parent
        0, 0,                           // x, y
        WIDTH, HEIGHT,                  // width, height
        0,                              // border width
        0x00000000,                     // border color
        0x00000000                      // background color
    );

	glXMakeCurrent(display, window, gl_context);

    XStoreName(display, window, "OpenGL");
    
    XSelectInput(display, window, KeyPressMask|KeyReleaseMask);
    
    XMapWindow(display, window);
    
    bool quit = false;
    while (!quit) {
        while (XPending(display) > 0) {
            XEvent event = {0};
            XNextEvent(display, &event);
            if (event.type == KeyPress) {
				KeySym keysym = XLookupKeysym(&event.xkey, 0);
				if (keysym == XK_Escape) {
                	quit = true;
				}
            }
        }
        
		glClearColor(0.15, 0.15, 0.15, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
			glColor3f(0.68, 0.84, 0.0);
			glVertex2f(-0.5, -0.5);
			glVertex2f( 0.5, -0.5);
			glVertex2f( 0.0,  0.5);
		glEnd();

		glXSwapBuffers(display, window);
    }
    
    XCloseDisplay(display);
}
