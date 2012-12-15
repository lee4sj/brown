varying vec3 light;
varying vec3 normal;

void main() 
{
    gl_TexCoord[0] =  gl_MultiTexCoord0;
    light = normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * gl_Vertex)).xyz;
    normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = ftransform();
}