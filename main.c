#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <SDL/SDL.h>

#include "vectorOps.h"
#include "triangle3d.h"
#include "icosahedron.h"
#include "shaderFunctions.h"
#include "misc.h"

#define WIDTH 640
#define HEIGHT 480

static SDL_Surface * gScreen;
static GLuint blur, blurV, combine;
static short timing = 1;
static int64_t start;
static float lastframe,thisframe;
static int ff, fps;

// textures
static GLuint screenTexture, blurTexture;
static GLuint texture;
static GLchar texturedata[256][4];


static GLvoid setDim(short dim) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	switch(dim) {
		case 2:
			glOrtho(0.0f, (float) WIDTH, (float) HEIGHT, 0.0f, -1.0f, 1.0f);
			break;
		case 3:
			glFrustum(-0.01, 0.01, -0.01 * (float) HEIGHT / (float) WIDTH, 0.01 * (float) HEIGHT / (float) WIDTH, 0.01, 1000.0);
			break;
		default:
			break;
	}
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static GLvoid drawQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(x, y);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x, y + h);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x + w, y + h);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x + w, y);
    }
    glEnd();
}

static GLvoid setLights() {
	// Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.6f, 0.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 0.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat position[] = { 0.0f, 0.0f, -4.0f, 0.0f };
	
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

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
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
//	glClearDepth(1.0f);
	GLchar * vsh = readGLSL("generic.vs");
	GLchar * fsh_blur = readGLSL("blur.fs");
	GLchar * fsh_blurV = readGLSL("blurV.fs");
	GLchar * fsh_combine = readGLSL("combine.fs");

    blur = makeProgram(vsh, fsh_blur, NULL);
	blurV = makeProgram(vsh, fsh_blurV, NULL);
    combine = makeProgram(vsh, fsh_combine, NULL);

	// generate texture
    int u, v, o = 0;
    unsigned char t;
	
    for (u = 0; u < 16; u++) {
        for (v = 0; v < 16; v++) {
            t = (unsigned char) (255.0f - fmin(32.0f * sqrt((7.5f - u) * (7.5f - u) + (7.5f - v) * (7.5f - v)), 255.0f));
            texturedata[o][0] = t;
            texturedata[o][1] = t;
            texturedata[o][2] = t;
            texturedata[o][3] = t;
            o++;
        }
    }
	
    // set texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, texturedata);
	
    makeTexture(&screenTexture);
    makeTexture(&blurTexture);   
	
	setLights();
	icosahedron();
	triangle3d();
	
	glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_SHADE_MODEL, GL_NICEST);
	
    // enable & setup pointsprites
    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glPointSize(8.0f);
    float pointDistanceAttenuation[] = {0.0f, 0.6f, 0.125f};
    glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, pointDistanceAttenuation);	
}

static void drawGL() {
	int p, q;
	double time = timing ? (utime() - start) / (1000.0 * 1000.0) : time;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setDim(3);
    glTranslatef(0, 0, -1.0);
	
    // particles
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    bindTexture(GL_TEXTURE0, texture);
	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glUseProgram(0);
    glDepthMask(0);
    glBegin(GL_POINTS);
    {
        int i, j;
        GLfloat f, y, b, vx, vy, vz;
		
        srand(0xC0DEBABE);
        for (j = 0; j < 300; j++) {
            f = j / 300.0f + time;
            vx = noise() * 0.75f;
            vy = 0.25f + fabs(noise()) * 0.5f;
            vz = noise() * 0.75f;
			
            for(i = 0; i < 32; i++) {
                y = frac(f + i / (31.0f * 4.0f));
                b = 1 - fabs(0.5f - y) / 0.5f;
                glColor4f(0.5f, 0.0f, 1.0f, 0.125f * b * i / 31.0f);
                glVertex3f(vx * y, -0.3f + vy * y - y * y * 0.25f, vz * y);
            }
        }
		
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
	
    glDepthMask(1);
    glLoadIdentity();
	
    glTranslatef(0.0, fabs(sin(time * 2 * 3.14159)), -6.0);
    glRotatef(time * 10, 2 * sin(0.5 * time) * cos(time), cos(time), sin(time));
    glRotatef(40 * time, 1.0, 0.0, 0.0);
    glRotatef(200 * time, 0.0, 1.0, 0.0);
    glScalef(1.0 + .3 * fabs(sin(time)), 1.0 + .3 * fabs(sin(time)), 1.0 + .3 * fabs(sin(time)));
	
    GLfloat mcolor[] = { 0.2f, 0.1f, 0.5l, 0.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
    glColor4f(0, 0, 0, 0);
    glCallList(1);
	
	// GLUquadricObj * quad = gluNewQuadric();
	// gluSphere(quad, 1.0, 32, 32);
	
    GLfloat m2color[] = { 0.1f, 0.6f, 0.1f, 0.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m2color);
	
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -6.0);
	
	glRotatef(150 * time, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 2.5);
	glRotatef(50 * time, 0.0, 1.0, 0.0);
	glRotatef(time * 10, 2 * sin(0.5 * time) * cos(time), cos(time), sin(time));
	glCallList(2);
	
    glDisable(GL_LIGHTING);
	
	// glow  
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
	
    setDim(2);
	
    bindTexture(GL_TEXTURE0, screenTexture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, WIDTH, HEIGHT, 0);
    bindTexture(GL_TEXTURE0, blurTexture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, WIDTH, HEIGHT, 0);
	
    glUseProgram(blur);
    // set blur strength
	// glUniform1f(glGetUniformLocation(blur, "strength"), 0.025);
    glUniform1f(glGetUniformLocation(blur, "radius"), 0.0035);
    // glUniform1f(glGetUniformLocation(blur, "vertical"), 1);
	glUniform1i(glGetUniformLocation(blur, "image"), 0);

	for(p = 3; p >= 0; p--) {
		drawQuad(0, 0, WIDTH, HEIGHT);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, WIDTH, HEIGHT, 0);
	}
	
	glUseProgram(blurV);
	// set blur strength
	// glUniform1f(glGetUniformLocation(blur, "strength"), 0.025);
    glUniform1f(glGetUniformLocation(blur, "radius"), 0.0035);
    // glUniform1f(glGetUniformLocation(blur, "vertical"), 1);
	glUniform1i(glGetUniformLocation(blur, "image"), 0);

	for(q = 3; q >= 0; q--) {
		drawQuad(0, 0, WIDTH, HEIGHT);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, WIDTH, HEIGHT, 0);
    }

	/*
	 bindTexture(GL_TEXTURE0, screenTexture);
	 bindTexture(GL_TEXTURE1, blurTexture);
	 
	 glUseProgram(combine);
	 glUniform1i(glGetUniformLocation(combine, "blur"), 1);
	 glUniform1i(glGetUniformLocation(combine, "screen"), 0);
	 drawQuad(0, 0, WIDTH, HEIGHT);
	 */
    glActiveTexture(GL_TEXTURE0);
	
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glUseProgram(0);
	
	thisframe = SDL_GetTicks();     // Count the FPS
	ff++;
	if((thisframe - lastframe) > 1000) {
		fps = ff;
		ff = 0;
		lastframe = thisframe;
		printf("%d\n", fps);
	}
	
	glFlush();
	SDL_GL_SwapBuffers();	
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
					switch(event.key.keysym.sym) {
						case SDLK_SPACE:
							timing = timing == 0 ? 1 : 0;
							start = utime();
							break;
						default:
							done = 1;
							break;
					}
					break;
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
	start = utime();
	lastframe = SDL_GetTicks();
    mainLoop();

    // Cleanup
	SDL_Quit();
	
    return EXIT_SUCCESS;
}
