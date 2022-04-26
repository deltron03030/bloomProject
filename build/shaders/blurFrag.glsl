/* #version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
    vec3 result = texture(screenTexture, TexCoords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
    // FragColor = texture(screenTexture, TexCoords).rgb;
}
*/

#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool horizontal;

// How far from the center to take samples from the fragment you are currently on
const int radius = 5;
// Keep it between 1.0f and 2.0f (the higher this is the further the blur reaches)
float spreadBlur = 2.0f;
float weights[radius];

void main()
{

    // Calculate the weights using the Gaussian equation

    float x = 0.0f;
    for (int i = 0; i < radius; i++)
    {
        // Decides the distance between each sample on the Gaussian function
        if (spreadBlur <= 2.0f)
        x += 3.0f / radius;
        else
        x += 6.0f / radius;

        weights[i] = exp(-0.5f * pow(x / spreadBlur, 2.0f)) / (spreadBlur * sqrt(2 * 3.14159265f));
    }


    vec2 tex_offset = 1.0f / textureSize(screenTexture, 0);
    vec3 result = texture(screenTexture, TexCoords).rgb * weights[0];

    // Calculate horizontal blur
    if(horizontal)
    {
        for(int i = 1; i < radius; i++)
        {
            // Take into account pixels to the right
            result += texture(screenTexture, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
            // Take into account pixels on the left
            result += texture(screenTexture, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
        }
    }
    // Calculate vertical blur
    else
    {
        for(int i = 1; i < radius; i++)
        {
            // Take into account pixels above
            result += texture(screenTexture, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weights[i];
            // Take into account pixels below
            result += texture(screenTexture, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weights[i];
        }
    }
    //FragColor = vec4(result, 1.0f);


    float max_radius = 8.0f;
    vec4 finalColor = vec4(0.0,0.0,0.0,1.0);
    float weight = 0.;//vec4(0.,0.,0.,0.);
    int radius_bokeh = int(max_radius);

    // computationally heavy bokeh blur version

/*
    for (int x=radius*-1; x<radius; x++) {
        for (int y=radius*-1; y<radius; y++){
            vec2 coord = TexCoords.xy + vec2(x,y) / textureSize(screenTexture, 0);
            if (distance(coord, TexCoords.xy) < float(radius)){
                vec4 texel = texture(screenTexture, coord);
                float w = length(texel.rgb)+0.1;
                weight+=w;
                finalColor += texel*w;
            }
        }
    }
*/

/*
    if(horizontal)
    {
        for(int x=radius*-1; x<radius; x++)
        {
            float x_coord = TexCoords.x + x / textureSize(screenTexture, 0).x;
            //if (abs(x_coord - TexCoords.x) < float(radius))
            if (distance(vec2(x_coord, TexCoords.y), TexCoords.xy) < float(radius))
            {
                vec4 texel = texture(screenTexture, TexCoords + vec2(x_coord, 0.0));
                float w = length(texel.rgb)+0.1;
                weight+=w;
                finalColor += texel*w;
            }
        }
    }
*/

    //finalColor += vec4(result, 1.0f);
    FragColor = vec4(result, 1.0f);
    // horizontal and vertical blur
/*
    if(horizontal)
    {
        for (int x=radius_bokeh*-1; x<radius_bokeh; x++) {
            for (int y=0; y<1; y++){
                vec2 coord = TexCoords.xy + vec2(x,y) / textureSize(screenTexture, 0);
                if (distance(coord, TexCoords.xy) < float(radius_bokeh)){
                    vec4 texel = texture(screenTexture, coord);
                    float w = length(texel.rgb)+0.1;
                    weight+=w;
                    finalColor += texel*w;
                }
            }
        }
    }
    // Calculate vertical blur
    else
    {
        for (int x=0; x<1; x++) {
            for (int y=radius_bokeh*-1; y<radius_bokeh; y++){
                vec2 coord = TexCoords.xy + vec2(x,y) / textureSize(screenTexture, 0);
                if (distance(coord, TexCoords.xy) < float(radius_bokeh)){
                    vec4 texel = texture(screenTexture, coord);
                    float w = length(texel.rgb)+0.1;
                    weight+=w;
                    finalColor += texel*w;
                }
            }
        }
    }

    FragColor = finalColor/weight;
*/
}







