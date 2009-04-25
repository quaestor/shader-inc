/*
 *  misc.h
 *  shader-inc
 *
 *  Created by Christoph on 4/24/09.
 *  Copyright 2009 FAU Erlangen-Nürnberg. All rights reserved.
 *
 */

#include <sys/types.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#endif

int64_t utime(void);
GLfloat frac(GLfloat);
GLfloat noise(void);
GLfloat getSmooth(GLfloat);
GLvoid makeTexture(GLuint *);
GLvoid bindTexture(GLuint, GLuint);