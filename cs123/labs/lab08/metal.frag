
varying vec3 vertex;		// The position of the vertex, in eye space
varying vec3 light;		// The normalized vector from the vertex to the light
varying vec3 eye;		// The normalized vector from the vertex to the eye
varying vec3 normal;		// The normal vector of the vertex, in eye space

uniform samplerCube envMap;	// The cube map containing the environment to reflect
uniform vec4 ambient;		// The ambient channel of the color to reflect
uniform vec4 diffuse;		// The diffuse channel of the color to reflect
uniform vec4 specular;		// The specular channel of the color to reflect
    
uniform float r0;		// The Fresnel reflectivity when the incident angle is 0
uniform float m;		// The roughness of the material

void main()
{
    vec3 e = normalize(eye);
    vec3 n = normalize(normal);
    vec3 i = normalize(vertex - eye);
    vec3 l = normalize(light);
    vec3 h = normalize(l - i);
    
    // TODO    
    // Compute diffuse coefficient
    float intensity = dot (n, l);
	
    // Add specular coefficient
    vec4 Q = vec4(reflect(-e, n), 0.0);
    Q = gl_ModelViewMatrixInverse * Q;

    float alpha = acos(dot(n,h));
    float D = exp(-pow(tan(alpha),2)/pow(m,2))/4/pow(m,2)/pow(cos(alpha),4);
    
    float G = min(1, 2*dot(h,n)*dot(e,n)/dot(e,h));
    G = min(G, 2*dot(h,n)*dot(l,n)/dot(e,h));
    
    float F = r0 + (1-r0)*pow((1-dot(e,n)), 5); // is the cos part correct?
    vec4 totalColor = intensity*diffuse + max(D*F*G/dot(e,n),0)*specular + ambient;

    gl_FragColor = textureCube(envMap, Q)*(F) + totalColor*(1-F);
}
//Need to find Q the incident vector of reflected light then multiply by F