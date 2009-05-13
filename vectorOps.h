/*
 *  vectorOps.h
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *
 */
#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#endif

GLvoid normalize(GLfloat[3]);
GLvoid normcrossprod(GLfloat[3], GLfloat[3], GLfloat[3]);
