/*
 *  triangle3d.c
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *  
 *
 */

#include "vectorOps.h"
#include "triangle3d.h"

GLuint tri = 2;

static GLfloat vdata[6][3] = {
{-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {0.0, 1.0, 0.0},
{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {0.0, 1.0, 1.0}
};

static GLint tindices[8][3] = {
{0, 2, 1},
{0, 3, 4}, {0, 4, 1},
{0, 5, 2}, {0, 3, 5},
{1, 2, 5}, {1, 5, 4},
{4, 5, 3}
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

GLvoid triangle3d() {
	glNewList(tri, GL_COMPILE);
	{
	//	glBegin(GL_TRIANGLE_STRIP);
//		glVertex3fv(vdata[0]); // A
//		glVertex3fv(vdata[2]); // C
//		glVertex3fv(vdata[1]); // B
//		glVertex3fv(vdata[4]); // E
//		glVertex3fv(vdata[0]); // A
//		glVertex3fv(vdata[3]); // D
//		glVertex3fv(vdata[2]); // C
//		glVertex3fv(vdata[5]); // F
//		glVertex3fv(vdata[4]); // E
//		glVertex3fv(vdata[3]); // D
//		glEnd();
		int i;
		for(i = 0; i < 8; i++) {
			drawtriangle(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]]);
		}
	}
	glEndList();
}