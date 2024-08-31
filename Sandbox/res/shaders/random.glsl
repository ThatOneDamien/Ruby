#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * vec4(a_Position, 1.0f);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_Color;


void main()
{
    o_Color = vec4(0.7f, 0.2f, 0.7f, 1.0f);
}
