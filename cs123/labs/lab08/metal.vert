
varying vec3 vertex;	// The position of the vertex, in eye space
varying vec3 light;	// The normalized vector from the vertex to the light
varying vec3 eye;	// The normalized vector from the vertex to the eye
varying vec3 normal;	// The normal vector of the vertex, in eye space

void main()
{
    vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
    light = normalize(gl_LightSource[0].position.xyz - vertex);
    eye = -vertex;
    normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = ftransform();
} 
