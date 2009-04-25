/*
 *  misc.h
 *  shader-inc
 *
 *  Created by Christoph on 4/24/09.
 *  Copyright 2009 FAU Erlangen-Nürnberg. All rights reserved.
 *
 */

#include <sys/types.h>
#include <OpenGL/OpenGL.h>

int64_t utime(void);
GLfloat frac(GLfloat);
GLfloat noise(void);
GLfloat getSmooth(GLfloat);
GLvoid makeTexture(GLuint *);
GLvoid bindTexture(GLuint, GLuint);