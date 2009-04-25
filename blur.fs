/*
 *  fsh_blur.glsl
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
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