
varying vec3 vertex;		// The position of the vertex, in eye space
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

void main() 
{
    vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
    normal = normalize(gl_NormalMatrix * gl_Normal);
    eye = -vertex;

    gl_Position = ftransform();
}
