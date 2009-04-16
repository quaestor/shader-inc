/*
 *  vectorOps.h
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *  Copyright 2009 FAU Erlangen-Nürnberg. All rights reserved.
 *
 */
#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#ifdef __linux__
#include <GL/gl.h>
#endif

GLvoid normalize(GLfloat[3]);
GLvoid normcrossprod(GLfloat[3], GLfloat[3], GLfloat[3]);
