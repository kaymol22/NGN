#version 460 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = a_Position;
}