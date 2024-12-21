#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProj;
};

layout(location = 1) out vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position;
    vec4 pos =  u_ViewProj * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 1) in vec3 v_TexCoords;

uniform samplerCube skybox;

void main()
{
    o_Color = texture(skybox, v_TexCoords);
}
