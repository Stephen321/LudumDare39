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
	a = 1 / (d / 900);
	a = a / 100.0;
	gl_FragColor.a = a;
}