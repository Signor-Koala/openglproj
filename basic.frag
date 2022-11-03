#version 460 core
out vec4 FragColor;

in float green;

void main() {

    FragColor = vec4(1.0, green, 1.0, 1.0);
}
