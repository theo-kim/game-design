#version 330 core
uniform sampler2D diffuse;
in vec2 texCoordVar;
out vec4 FragColor;

void main() {
    FragColor = texture(diffuse, texCoordVar);
}
