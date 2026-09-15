#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
	vec4 worldPos = u_Transform * vec4(vPosition, 1.0);
	v_FragPos = worldPos.xyz;
	v_Normal = normalize(u_NormalMatrix * a_Normal);
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * worldPos;
}