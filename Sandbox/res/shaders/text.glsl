#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;

out vec4 v_Color;
out vec2 v_TexCoords;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProj;
};

void main()
{
	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
	v_Color = a_Color;
	v_TexCoords = a_TexCoords;
}

#shader fragment
#version 450 core

layout(binding = 0) uniform sampler2D u_Tex;

in vec4 v_Color;
in vec2 v_TexCoords;

void main()
{
    float color = texture(u_Tex, v_TexCoords).r;
	gl_FragColor = v_Color * vec4(1.0f, 1.0f, 1.0f, color);
}