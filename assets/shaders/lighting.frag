#version 120

uniform sampler2D texture;
uniform sampler2D lightmask;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 lightpixel = texture2D(lightmask, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel * lightpixel;
}