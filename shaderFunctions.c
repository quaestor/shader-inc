/*
 *  shaderFunctions.c
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "shaderFunctions.h"

GLchar * readGLSL(const char * path) {
	FILE * fp;
	char * content = NULL;
	
	int count = 0;
	
	if (path != NULL) {
		fp = fopen(path, "rt");
		
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
			
		}
	}
	
	return content;
}

GLvoid printShaderInfo(GLuint obj) {
    GLint infologLength = 0;
    GLint charsWritten  = 0;
    GLchar * infoLog;
	
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
    if(infologLength > 1) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        if(infologLength > 0) {
            printf("\n%s",infoLog);
        };
        free(infoLog);
    } else {
        printf(" [ok]\n");
    }
}

GLvoid printProgramInfo(GLuint obj) {
	int infologLength = 0;
	int charsWritten  = 0;
	char * infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if(infologLength > 0) {
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("printProgramInfoLog: %s\n",infoLog);
		free(infoLog);
	} else {
	 	printf("Program Info Log: OK\n");
	}
}

GLuint makeProgram(GLchar * v, GLchar * f, GLchar * g) {
    GLuint program, vertex, fragment, geometry;
	
    vertex   = v == NULL ? 0 : glCreateShader(GL_VERTEX_SHADER);
    fragment = f == NULL ? 0 : glCreateShader(GL_FRAGMENT_SHADER);
	geometry = g == NULL ? 0 : glCreateShader(GL_GEOMETRY_SHADER_EXT);
	
    if(vertex)   glShaderSource(vertex, 1, (const char**)&v, NULL);
    if(fragment) glShaderSource(fragment, 1, (const char**)&f, NULL);
	if(geometry) glShaderSource(geometry, 1, (const char **)&g, NULL);
	
    if(vertex)   glCompileShader(vertex);
    if(fragment) glCompileShader(fragment);
	if(geometry) glCompileShader(geometry);
	
	
//#ifdef DEBUG
	if(vertex) {
		printf("\t> Vertex Shader Info:");
		printShaderInfo(vertex);
	}
	if(fragment) {
		printf("\t> Fragment Shader Info:");
		printShaderInfo(fragment);
	}
	if(geometry) {
		printf("\t> Geometry Shader Info:");
		printShaderInfo(geometry);
	}
//#endif
	
    program = glCreateProgram();
	
    if(vertex)   glAttachShader(program, vertex);
    if(fragment) glAttachShader(program, fragment);
	if(geometry) glAttachShader(program, geometry);
	
	if(geometry) {
		glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
		glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
		
		int temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
		glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, temp);
	}
    glLinkProgram(program);
	
#ifdef DEBUG
	printProgramInfo(program);
#endif
	
    return program;
}
