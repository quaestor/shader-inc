/*
 *  vsh_generic.glsl
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *  Copyright 2009 FAU Erlangen-Nürnberg. All rights reserved.
 *
 */
uniform sampler2D MyTex;
void main() {
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}