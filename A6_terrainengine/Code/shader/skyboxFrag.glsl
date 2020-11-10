#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D skybox;
uniform vec2 texTranslate;

void main()
{
    FragColor = texture(skybox, texTranslate + TexCoords); //vec4(TexCoords + 1.0, 1.0);
}