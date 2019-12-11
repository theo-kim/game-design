#version 330 core
uniform vec4 color;
uniform float lightPower;
uniform vec3 lightPosition;
uniform vec3 lightColor;

in vec3 Normal_camera;
in vec3 LightDirection_camera;
in vec3 Position_global;
in vec3 EyeDirection_camera;

out vec4 FragColor;

void main() {
  vec3 n = normalize(Normal_camera);
  vec3 l = normalize(lightPosition - Position_global);
  float cosTheta = clamp(dot(n, l), 0.0, 1.0);
  float distance = length( lightPosition - Position_global );
  
  // Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_camera);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0.0, 1.0);
  float specularStrength = 0.3;
  float shininess = 16.0;
  float ambientStrength = 0.1;
  float sqDistance = distance * distance;

  vec3 MaterialAmbientColor = ambientStrength * lightColor;
  //* lightPower / sqDistance;  
  vec3 MaterialDiffuseColor = lightColor * lightPower * cosTheta / sqDistance;
  vec3 MaterialSpecularColor = specularStrength * lightColor * lightPower * pow(cosAlpha, shininess) / sqDistance;
  
  vec4 c = color * vec4(MaterialAmbientColor + MaterialDiffuseColor + MaterialSpecularColor, 1);
  FragColor = c;
}
