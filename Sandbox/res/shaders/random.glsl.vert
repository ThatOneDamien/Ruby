#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
// layout(location = 2) in vec2 a_UV;

uniform mat4 u_Proj;

out vec3 v_Color;
out vec3 v_Normal;

void main()
{
    gl_Position = u_Proj * vec4(a_Position, 1.0f);
    v_Color = a_Normal;
    v_Normal = normalize(a_Position);
}
