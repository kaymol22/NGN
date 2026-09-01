#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
	v_FragPos = worldPos.xyz;
	v_Normal = normalize(u_NormalMatrix * a_Normal);
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * worldPos;
}

#type fragment
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