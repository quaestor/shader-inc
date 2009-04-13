/*
 *  shaderFunctions.h
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *  Copyright 2009 FAU Erlangen-Nürnberg. All rights reserved.
 *
 */
#include <OpenGL/gl.h>

GLchar * readGLSL(char *);
GLvoid printShaderInfo(GLuint);
GLvoid printProgramInfo(GLuint);
GLuint makeProgram(GLchar *, GLchar *, GLchar *);