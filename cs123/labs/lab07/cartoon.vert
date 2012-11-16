varying float intensity;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec3 n, l;
    n = vec3(gl_NormalMatrix * gl_Normal);
    l = vec3(normalize(gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex));
    
    intensity = dot(n, l);
}
