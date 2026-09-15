#version 460 core

layout(binding = 0) uniform sampler2D u_Texture;

layout(location = 0) in vec2 v_UV;
layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(u_Texture, vec2(v_UV.x, 1- v_UV.y));
	outColor = texture(u_Texture, vec2(v_UV.x, v_UV.y));
}