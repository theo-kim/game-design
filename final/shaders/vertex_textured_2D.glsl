#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 texCoordVar;

void main()
{
	vec4 p = viewMatrix * modelMatrix  * position;
    texCoordVar = texCoord;
	gl_Position = projectionMatrix * p;
}