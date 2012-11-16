
varying vec3 vertex;		// The position of the vertex, in eye space
varying vec3 light;		// The normalized vector from the vertex to the light
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

uniform float r0;		// The R0 value to use in Schlick's approximation
uniform float eta1D;		// The eta value to use initially
uniform vec3  eta;		// Contains one eta for each channel (use eta.r, eta.g, eta.b in your code)

uniform samplerCube envMap;

void main()
{
    vec3 n = normalize(normal);
    vec3 e = normalize(vertex);
    vec3 i = normalize(vertex - eye);
    vec3 refractR = refract(e, n, eta.r);
    vec3 refractG = refract(e, n, eta.g);
    vec3 refractB = refract(e, n, eta.b);
    vec3 t1 = gl_ModelViewMatrixInverse *  vec4(refractR,0);
    vec3 t2 = gl_ModelViewMatrixInverse *  vec4(refractG,0);
    vec3 t3 = gl_ModelViewMatrixInverse *  vec4(refractB,0);

    vec3 e2 = normalize(eye);
    vec3 l = normalize(light);
    vec3 h = normalize(l - i);
    
    //COPY PASTE
    vec4 Q = vec4(reflect(-e2, n), 0.0);
    float F = r0 + (1-r0)*pow((1-dot(e2,n)), 5); // is the cos part correct?
	
    float red = textureCube ( envMap , t1).r;
    float green = textureCube ( envMap , t2).g;
    float blue = textureCube ( envMap , t3).b;
    
    // z = a * x + b * y
    // a + b = 1
    // z  a * x + (1 - a) * y
    
    gl_FragColor = vec4( red, green, blue, 1.0)*(1-F) + textureCube(envMap, Q)*F;
}
