varying float intensity;
varying float height;

void main()
{
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    vec3 light = normalize(gl_LightSource[0].position - (gl_ModelViewMatrix * gl_Vertex)).xyz;
    intensity = max(0.0, dot(normal, light));
    
    gl_Position = ftransform();
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    height = gl_Vertex.z;
}
