uniform sampler2D treeTexture;
uniform sampler2D normalMap;
uniform float bumpIntensity;
uniform float bumpOn;

varying vec3 light;
varying vec3 normal;

void main()
{
    //Get Texture
    vec4 tTexture = texture2D(treeTexture, gl_TexCoord[0].st);

    //get intensity
    vec3 curNormal = normalize(texture2D(normalMap, gl_TexCoord[0].st)).xyz;
    curNormal = normalize(normal + bumpIntensity * curNormal);
    float intensity = max(0.0, dot(curNormal, light));
    intensity = min(1.0, intensity);

    gl_FragColor = tTexture * intensity;
    gl_FragColor.w = 1.0;
}
