#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D emmisionTexture;
uniform vec3 Color;

void main()
{
    FragColor = texture(emmisionTexture, TexCoords) * vec4(Color, 1.0);
}