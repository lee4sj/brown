uniform sampler2D leafTexture;

varying vec3 light;
varying vec3 normal;

void main()
{
    //Get Texture
    vec4 tTexture = texture2D(leafTexture, gl_TexCoord[0].st);

    //get intensity
    float intensity = max(0.2, dot(normal, light));
    intensity = min(1.0, intensity);

    if (tTexture.x >= 0.9 && tTexture.y >= 0.9 && tTexture.z >= 0.9)
        gl_FragColor = vec4(0,0,0,0);
    else {
        gl_FragColor = tTexture * intensity;
        gl_FragColor.w = 1.0;
    }
}
