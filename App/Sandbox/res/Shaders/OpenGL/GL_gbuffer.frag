#version 460 core

in vec3 Normal;
in vec4 WorldPos;
in flat vec4 Color;

layout(location = 0) out vec4 BaseColorMetallic;
layout(location = 1) out vec4 NormalRoughness;

void main()
{
	BaseColorMetallic = vec4(Color.rgb, 0.0);
	NormalRoughness = vec4(normalize(Normal), 0.0);
}