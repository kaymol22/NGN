#type vertex
#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPos;

void main()
{
    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal = u_NormalMatrix * a_Normal;
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{
    // Simple diffuse lighting from top
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(v_Normal);
    float diffuse = max(dot(normal, lightDir), 0.2);

    FragColor = u_Color * diffuse;
}
