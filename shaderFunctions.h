/*
 *  shaderFunctions.h
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

GLchar * readGLSL(const char *);
GLvoid printShaderInfo(GLuint);
GLvoid printProgramInfo(GLuint);
GLuint makeProgram(GLchar *, GLchar *, GLchar *);
