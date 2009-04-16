#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#include <SDL/SDL.h>

#define WIDTH 640
#define HEIGHT 480

static SDL_Surface * gScreen;

static void initAttributes() {
    // Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
    //    Mac OS X will always use 8-8-8-8 ARGB for 32-bit screens and
    //    5-5-5 RGB for 16-bit screens

    // Request a 16-bit depth buffer (without this, there is no depth buffer)
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    // Request double-buffered OpenGL
    //     The fact that windows are double-buffered on Mac OS X has no effect
    //     on OpenGL double buffering.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

static void printAttributes() {
    // Print out attributes of the context we created
    int nAttr;
    int i;

    int attr[] = { SDL_GL_RED_SIZE, SDL_GL_BLUE_SIZE, SDL_GL_GREEN_SIZE,
                   SDL_GL_ALPHA_SIZE, SDL_GL_BUFFER_SIZE, SDL_GL_DEPTH_SIZE };

    char * desc[] = { "Red size: %d bits\n", "Blue size: %d bits\n", "Green size: %d bits\n",
                      "Alpha size: %d bits\n", "Color buffer size: %d bits\n", 
                      "Depth bufer size: %d bits\n" };

    nAttr = sizeof(attr) / sizeof(int);

    for(i = 0; i < nAttr; i++) {
        int value;
        SDL_GL_GetAttribute(attr[i], &value);
        printf(desc[i], value);
    } 
}

static void createSurface(int fullscreen) {
    Uint32 flags = 0;

    flags = SDL_OPENGL;
    if(fullscreen) {
        flags |= SDL_FULLSCREEN;
    }

    // Create window
    gScreen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, flags);
    if(gScreen == NULL) {
        fprintf(stderr, "Couldn't set %dx%d OpenGL video mode: %s\n", WIDTH, HEIGHT, SDL_GetError());
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
}

static void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
}

static void drawGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void mainLoop() {
	SDL_Event event;
	int done = 0;

    while(!done) {
		/* Check for events */
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					/* Any keypress quits the app... */
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}

        drawGL();
        SDL_GL_SwapBuffers();
	}
}

int main(int argc, char ** argv) {
	// Init SDL video subsystem
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

    // Set GL context attributes
    initAttributes();    
    // Create GL context
    createSurface(0);
    // Get GL context attributes
    printAttributes();
    // Init GL state
    initGL();
    // Draw, get events...
    mainLoop();

    // Cleanup
	SDL_Quit();
	
    return EXIT_SUCCESS;
}