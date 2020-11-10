#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 clipPlane;

void main()
{
    TexCoords = aTexCoords;
    vec4 Position = vec4(aPos, 1.0);
    gl_Position = projection * view * model * Position;
    gl_ClipDistance[0] = dot(model * Position, clipPlane);
}