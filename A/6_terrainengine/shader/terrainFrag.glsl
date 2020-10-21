#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D terrainTexture;
uniform sampler2D detailTexture;

void main()
{
    vec4 terrainColor = texture(terrainTexture, TexCoords);
    vec4 detailColor = texture(detailTexture, TexCoords);
    FragColor = terrainColor + detailColor - 0.5;
}