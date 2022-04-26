#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool horizontal;

void main()
{
    float max_radius = 8.0f;
    vec4 finalColor = vec4(0.0,0.0,0.0,1.0);
    float weight = 0.;//vec4(0.,0.,0.,0.);
    int radius = int(max_radius);

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
    // horizontal and vertical blur

    if(horizontal)
    {
        for (int x=radius*-1; x<radius; x++) {
            for (int y=0; y<1; y++){
                vec2 coord = TexCoords.xy + vec2(x,y) / textureSize(screenTexture, 0);
                if (distance(coord, TexCoords.xy) < float(radius)){
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
    }

    FragColor = finalColor/weight;

}