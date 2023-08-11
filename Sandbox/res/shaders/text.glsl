#shader vertex
#version 410 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoords;

uniform mat4 u_ViewProj;
out vec2 v_TexCoords;

void main()
{
	gl_Position = u_ViewProj * vec4(a_Position.xy, 0.0, 1.0);
	v_TexCoords = a_TexCoords;
}

#shader fragment
#version 410 core

uniform sampler2D u_Tex;

in vec2 v_TexCoords;

void main()
{
    float color = texture(u_Tex, v_TexCoords).r;
	gl_FragColor = vec4(1.0f, 1.0f, 1.0f, color);
}