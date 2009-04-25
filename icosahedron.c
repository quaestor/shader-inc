/*
 *  icosahedron.c
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *
 */

#include "vectorOps.h"
#include "icosahedron.h"

#define X .525731112119133606
#define Z .850650808352039932

GLuint ico = 1;

static GLfloat vdata[12][3] = {
{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

static GLint tindices[20][3] = {
{0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1},
{8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3},
{7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
{6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}
};

static GLvoid drawtriangle(GLfloat * v1, GLfloat * v2, GLfloat * v3) { 
	glBegin(GL_TRIANGLES); 
	{
		int j;
		GLfloat d1[3], d2[3], norm[3]; 
		for(j = 0; j < 3; j++) { 
			d1[j] = v1[j] - v2[j]; 
			d2[j] = v2[j] - v3[j]; 
		}
		
		normcrossprod(d1, d2, norm);
		
		glNormal3fv(norm);
		//glNormal3fv(v1); 
		glVertex3fv(v1);
		//glNormal3fv(v2); 
		glVertex3fv(v2);
		//glNormal3fv(v3); 
		glVertex3fv(v3);
	}
	glEnd(); 
}

static GLvoid subdivide(GLfloat * v1, GLfloat * v2, GLfloat * v3, GLuint depth) {
	GLfloat v12[3], v23[3], v31[3];
	GLint i;
	
	if(!depth) {
		drawtriangle(v1, v2, v3);
		return;
	}
	for(i = 0; i < 3; i++) {
		v12[i] = (v1[i] + v2[i]) / 2.0f;
		v23[i] = (v2[i] + v3[i]) / 2.0f;
		v31[i] = (v3[i] + v1[i]) / 2.0f;
	}
	
	normalize(v12);
	normalize(v23);
	normalize(v31);
	
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}

GLvoid icosahedron() {
	glNewList(ico, GL_COMPILE);
	{
		int i;
		for(i = 0; i < 20; i++) {
			subdivide(vdata[tindices[i][0]],       
					  vdata[tindices[i][1]],       
					  vdata[tindices[i][2]], 1);
		}
	}
	glEndList();
}