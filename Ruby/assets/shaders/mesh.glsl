#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_MatIndex;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProj;
};

layout(std140, binding = 1) uniform Models
{
    mat4 u_ModelArr[100];
};

layout(location = 5) out vec4 v_Position;
layout(location = 6) out vec4 v_Normal;
layout(location = 7) out flat float v_MatIndex;

void main()
{
    v_Normal = vec4(a_Normal, 0.0f);
    v_MatIndex = a_MatIndex;
    v_Position = u_ModelArr[gl_InstanceID] * vec4(a_Position, 1.0f);
    gl_Position = u_ViewProj * v_Position;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 5) in vec4 v_Position;
layout(location = 6) in vec4 v_Normal;
layout(location = 7) in flat float v_MatIndex;

struct Material
{
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    float SpecularHl;
    float LightModel;
};

layout(std140, binding = 2) uniform MatThing
{
    Material u_Mats[16];
};

layout(std140, binding = 3) uniform Light
{
    vec4 u_LightPos;
    vec4 u_LookerPos;
};

void main()
{
    int index = int(v_MatIndex);
    if(u_Mats[index].LightModel < 0.0f)
        discard;
    vec4 lightDir = normalize(u_LightPos - v_Position);
    vec4 normal = normalize(v_Normal);
    float angle = dot(normal, lightDir);
    vec4 color = vec4(0.0f);
    if(u_Mats[index].LightModel == 0.0f)
        color = u_Mats[index].Diffuse;
    else
    {
        color = 0.1f * u_Mats[index].Ambient + max(angle * 0.7f + 0.3f, 0.0f) * u_Mats[index].Diffuse; 
        if(u_Mats[index].LightModel >= 2.0f)
        {
            vec4 reflected = normalize((2.0f * angle) * normal);
            vec4 toObs = normalize(u_LookerPos - v_Position);
            color += u_Mats[index].Specular * pow(max(dot(reflected, toObs), 0.0f), u_Mats[index].SpecularHl);
        }
    }
    o_Color = vec4(color.xyz, 1.0f);
}
