uniform float radius; 
uniform sampler2D image;
float ypos;
vec2 pos = gl_TexCoord[0].xy;

void main()
{
	vec4 color = texture2D(image, pos);
	for(float i = 0.; i <= radius * 2.; i++) {
		ypos = i - radius;
		color += texture2D(image, vec2(pos.x, pos.y + ypos));
	}
	color.r = (color.r - 0.05) * 1.2;
	color.g = (color.g - 0.05) * 1.2;
//	color.b = (color.b - 0.05) * 1.2;
	gl_FragColor = color / (radius * 2. + 2.);
}