#version 460 core

in vec3 FragPos;
in vec2 Normal;
in vec2 TexCoord;

uniform vec4 u_Color;

layout(location = 0) out vec4 BaseColor+Config;
layout(location = 1) out vec4 Lighting;

void main()
{
	BaseColor+Config = u_Color;
}