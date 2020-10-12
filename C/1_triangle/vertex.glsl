#version 330 core
layout (location = 0) in vec3 aPoS;

void main()
{
	gl_Position = vec4(aPoS, 1.0);
}