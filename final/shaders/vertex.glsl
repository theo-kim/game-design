#version 330 core
in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	vec4 p = viewMatrix * modelMatrix  * position;
	gl_Position = projectionMatrix * p;
	float ZNEAR = 0.0001;
	float ZFAR = 1000000.0;
	float FCOEF = 2.0 / log2(ZFAR + 1.0);
	gl_Position.z = log2(max(ZNEAR, 1.0 + gl_Position.w)) * FCOEF - 1.0;
}
