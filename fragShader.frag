#version 460 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord0;
in vec2 TexCoord1;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float time;
uniform mat4 transform;

void main()
{
    FragColor = mix(texture(texture0, TexCoord0), texture(texture1, TexCoord1), sin(time)) * ourColor;
}