#shader vertex
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

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Color;
in vec3 v_Normal;
uniform vec3 u_LightDir;
uniform vec3 u_LightColor;

void main()
{
    vec3 lightColor = u_LightColor * (dot(v_Normal, u_LightDir) + vec3(1.0f)) / 2.0f;
    o_Color = vec4(lightColor * v_Color, 1.0f);
}
