#version 120

uniform sampler2D texture;
uniform vec2 lightPosition;
varying vec2 position;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	float d = distance(lightPosition, position);

    // multiply it by the color

	gl_FragColor = gl_Color * pixel;
	
	
	float a = 1;
	if (d > 170)
		a = 0;
	else
		a = 1 - (d / 170);
	gl_FragColor.a = a;
}