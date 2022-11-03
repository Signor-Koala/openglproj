#version 460 core
layout (location = 0) in vec3 aPos;

out float green;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    green = aPos.y*time*1.5;
}
