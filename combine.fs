/*
 *  fsh_combine.glsl
 *  Isokaeder
 *
 *  Created by Christoph on 3/25/09.
 *
 */

uniform sampler2D blur;
uniform sampler2D screen;

void main() {
	vec4 m = texture2D(blur, gl_TexCoord[0].xy);
	vec4 n = texture2D(screen, gl_TexCoord[0].xy);
	gl_FragColor = n + m.a * m * 2.;
}