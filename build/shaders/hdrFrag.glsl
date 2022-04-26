#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float exposure;

const float offset = 1.0 / 600.0;
const float gamma = 2.2;

void main()
{
    // kernal effect implementation

    vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
    -2, -2, -2,
    -2,  17, -2,
    -2, -2, -2
    //1.0 / 16, 2.0 / 16, 1.0 / 16,
    //2.0 / 16, 4.0 / 16, 2.0 / 16,
    //1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }
    // FragColor = vec4(col, 1.0);


    // HDR color calculation
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    // reinhard tone mapping
    // vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // exposure tone mapping
    // vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    vec3 mapped = vec3(1.0) - exp(-hdrColor * 0.6);
    // gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));
    FragColor = vec4(mapped, 1.0);



    //vec3 col = texture(screenTexture, TexCoords).rgb;
    //FragColor = vec4(col, 1.0);
}