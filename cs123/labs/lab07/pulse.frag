varying float intensity;

uniform vec4 color;

void main()
{	
    gl_FragColor = color * intensity;
}
