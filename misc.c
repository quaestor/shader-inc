/*
 *  misc.c
 *  shader-inc
 *
 *  Created by Christoph on 4/24/09.
 *  Copyright 2009 FAU Erlangen-Nürnberg. All rights reserved.
 *
 */

#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#include "misc.h"

int64_t utime() {
    static struct timeval value;
	
    gettimeofday(&value, NULL);
    return (int64_t) value.tv_sec * 1e6 + (int64_t) value.tv_usec;
}

GLfloat frac(GLfloat f) {
    return f - (int)f;
}

GLfloat noise() {
    return ((GLfloat) rand() / (GLfloat) RAND_MAX - 0.5f) * 2.0f;
}

GLfloat getSmooth(GLfloat f) {
    return 1.0f - fabs(f - 0.5f) * 2.0f;
}

GLvoid makeTexture(GLuint * texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GLvoid bindTexture(GLuint index, GLuint texture) {
    glActiveTexture(index);
    glBindTexture(GL_TEXTURE_2D, texture);
}
