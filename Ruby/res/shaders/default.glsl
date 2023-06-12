#shader vertex
#version 410 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
	gl_Position = vec4(a_Position.xy, 0.0, 1.0);
	v_TexCoords = a_TexCoords;
}

#shader fragment
#version 410 core

in vec2 v_TexCoords;

uniform sampler2D u_Tex;

void main()
{
	gl_FragColor = texture(u_Tex, v_TexCoords);
}