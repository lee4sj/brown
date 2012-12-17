const int MAX_KERNEL_SIZE = 128;
uniform sampler2D tex;
uniform int arraySize;
uniform vec2 offsets[MAX_KERNEL_SIZE]; 
uniform float kernel[MAX_KERNEL_SIZE];
void main(void) { 
    // Fill this in!
    vec4 color = vec4(0,0,0,0);
    for(int i =0; i<arraySize; i++)
    {
        vec2 coord = gl_TexCoord[0].st + offsets[i];
	color += texture2D(tex, coord)*kernel[i];
    }
    gl_FragColor = color;
}
