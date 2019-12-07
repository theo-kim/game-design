#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform vec3 lightPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix; 
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

out vec3 Normal_camera;
out vec3 LightDirection_camera;
out vec3 Position_global;
out vec3 EyeDirection_camera;

void main()
{
  // vec3 lightPosition = vec3(1, 1, 0);
  // Position of the vertex, in worldspace : modelMatrix * position
  Position_global = (modelMatrix * vec4(position, 1)).xyz;

  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 Position_camera = (viewMatrix * vec4(Position_global, 1)).xyz;
  EyeDirection_camera = (vec4(0, 0, 0, 0) - vec4(Position_camera, 1)).xyz;

  // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
  vec3 LightPosition_camera = (viewMatrix * vec4(lightPosition, 1)).xyz;
  LightDirection_camera = LightPosition_camera + EyeDirection_camera;

  // Normal of the the vertex, in camera space
  Normal_camera = (modelMatrix * vec4(normal, 0)).xyz;
  // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not

  gl_Position = projectionMatrix * vec4(Position_camera, 1);
  float ZNEAR = 0.0001;
	float ZFAR = 1000000.0;
	float FCOEF = 2.0 / log2(ZFAR + 1.0);
	gl_Position.z = log2(max(ZNEAR, 1.0 + gl_Position.w)) * FCOEF - 1.0;
}