#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_UV;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProj;
};

layout(std140, binding = 1) uniform Models
{
    mat4 u_ModelArr[100];
};

layout(location = 4) out vec4 v_Color;

void main()
{
    gl_Position = u_ViewProj * u_ModelArr[gl_InstanceID] * vec4(a_Position, 1.0f);
    v_Color = a_Color;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 4) in vec4 v_Color;

void main()
{
    o_Color = v_Color;
}
