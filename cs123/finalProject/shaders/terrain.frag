uniform sampler2D leafTexture;

varying vec3 light;
varying vec3 normal;

void main()
{
    //Get Texture
    vec4 tTexture = texture2D(leafTexture, gl_TexCoord[0].st);

    //get intensity
    float intensity = max(0.0, dot(normal, light));

    gl_FragColor = tTexture * intensity;
}
