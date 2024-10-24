#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProj;
};

layout(location = 2) out flat vec4 v_Color;

void main()
{
    gl_Position = u_ViewProj * vec4(a_Position, 1.0f);
    v_Color = vec4(a_Color, 1.0f);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 2) in flat vec4 v_Color;

void main()
{
    o_Color = v_Color;
}
