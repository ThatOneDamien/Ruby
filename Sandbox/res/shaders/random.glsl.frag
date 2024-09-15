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
