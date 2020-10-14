#version 330 core
layout (location = 0) in vec3 aPoS;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 transform;

out vec2 TexCoords;

void main()
{
    gl_Position = transform * vec4(aPoS, 1.0);
    TexCoords = aTexCoords;
}