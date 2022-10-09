#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord0;
out vec2 TexCoord1;

uniform vec4 rainbowColour;
uniform mat4 transform;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = rainbowColour;
    TexCoord0 = aTexCoord;
    TexCoord1 = vec2(vec4(aTexCoord, 0.0f, 1.0f) * transform);
}
