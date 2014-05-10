#version 410

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D vTexCoord;

void main()
{
    outColor = texture(vTexCoord, TexCoord).rgba;
}