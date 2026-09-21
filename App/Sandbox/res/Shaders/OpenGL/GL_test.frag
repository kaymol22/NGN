#version 460 core

layout(location = 0) out vec4 BaseColorMetallic;
layout(location = 1) out vec4 NormalRoughness;

void main()
{
    BaseColorMetallic = vec4(1.0, 0.0, 1.0, 1.0); // magenta — unmistakable if it shows up
    NormalRoughness = vec4(0.0, 0.0, 1.0, 0.0);
}