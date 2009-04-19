/*
 *  vectorOps.c
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *
 */

#include <math.h>
#include <stdio.h>

#include "vectorOps.h"

GLvoid normalize(GLfloat v[3]) { 
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if(d == 0.0) {
        printf("warning: zero length vector"); 
        return;
    }
    v[0] /= d; v[1] /= d; v[2] /= d;
}

GLvoid normcrossprod(GLfloat v1[3], GLfloat v2[3], GLfloat out[3]) {
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
    normalize(out);
}
