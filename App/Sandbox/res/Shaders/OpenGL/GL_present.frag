#version 460 core

in vec3 v_FragPos;
in vec2 v_Normal;
in vec2 v_TexCoord;

uniform vec4 u_Color;

layout(location = 0) out vec4 g_Albedo;

void main()
{
	// Raw albedo for now
	g_Albedo = u_Color;
}