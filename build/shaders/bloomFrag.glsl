#version 330

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform sampler2D bloomTexture2;
uniform sampler2D bloomTexture3;
uniform float exposure;

void main() {
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
    vec3 bloomColor2 = texture(bloomTexture2, TexCoords).rgb;
    vec3 bloomColor3 = texture(bloomTexture3, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
    hdrColor += bloomColor2;
    //hdrColor += bloomColor3;
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}
