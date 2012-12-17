uniform sampler2D leafTexture;

varying vec3 light;
varying vec3 normal;

void main()
{
    //Get Texture
    vec4 tTexture = texture2D(leafTexture, gl_TexCoord[0].st);

    //get intensity
    float intensity = max(0.0, dot(normal, light));

    if (tTexture.x >= 0.9 && tTexture.y >= 0.9 && tTexture.z >= 0.9)
        tTexture = vec4(0,0,0,0);

    gl_FragColor = tTexture * intensity;
}
