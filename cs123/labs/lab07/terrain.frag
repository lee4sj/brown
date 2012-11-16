
uniform float dirtMin, dirtMax;
uniform float grassMin, grassMax;
uniform float rockMin, rockMax;
uniform float snowMin, snowMax;
    
uniform sampler2D dirtTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

varying float height;
varying float intensity;

float getWeight(float rmin, float rmax)
{
    float range = rmax - rmin;
    float w = (range - abs(height - rmax)) / range;
    w = max(0.0, w);
    
    return w;
}

vec4 sampleTextures()
{
    float dirtWeight = getWeight(dirtMin, dirtMax);
    float grassWeight = getWeight(grassMin, grassMax);
    float rockWeight = getWeight(rockMin, rockMax);
    float snowWeight = getWeight(snowMin, snowMax);
    
    vec4 dirtSample = dirtWeight * texture2D(dirtTexture, gl_TexCoord[0].st);
    vec4 grassSample = grassWeight * texture2D(grassTexture, gl_TexCoord[0].st);
    vec4 rockSample = rockWeight * texture2D(rockTexture, gl_TexCoord[0].st);
    vec4 snowSample = snowWeight * texture2D(snowTexture, gl_TexCoord[0].st);
    
    return dirtSample + grassSample + rockSample + snowSample;
}

void main()
{   
    gl_FragColor = sampleTextures() * intensity;
}
