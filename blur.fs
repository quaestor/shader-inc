/*
 *  fsh_blur.glsl
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *  Copyright 2009 Christoph Rauch
 *
 */

uniform float radius; 
uniform sampler2D image;
float xpos;
vec2 pos = gl_TexCoord[0].xy;

void main()
{
	vec4 color = texture2D(image, pos);
	for (float i = 0.; i <= radius * 2.; i++) {
		xpos = i - radius;
		color += texture2D(image, vec2(pos.x + xpos, pos.y));
	}
	color.a = color.b;
	gl_FragColor = color / (radius * 2. + 2.);
}

/* LAS BLUR */
/*
uniform float radius;
uniform sampler2D image;
void main() {
	vec4 m = texture2D(image, gl_TexCoord[0].xy);
	vec2 n;
	for(int i = 0; i < 8; i++) {
		float j = float(i);
		n.x = cos(j * 0.75);
		n.y = sin(j * 0.75);
		m += texture2D(image, gl_TexCoord[0].xy + radius * n);
	}
	gl_FragColor = m / 8.0;
}
*/