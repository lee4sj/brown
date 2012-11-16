varying float intensity;

uniform float threshold1, threshold2, threshold3;
uniform vec4 color1, color2, color3, color4;

void main()
{
    if (intensity < threshold3)
	gl_FragColor = color1;
    else if (intensity < threshold2)
	gl_FragColor = color2;
    else if (intensity < threshold1)
	gl_FragColor = color3;
    else
	gl_FragColor = color4;
}