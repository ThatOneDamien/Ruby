#shader vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex;

void main()
{
	gl_Position = vec4(a_Position.xy, 0.0, 1.0);
	v_Color = a_Color;
	v_TexCoords = a_TexCoords;
	v_TexIndex = a_TexIndex;
}

#shader fragment
#version 410 core

in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	gl_FragColor = texture(u_Textures[index], v_TexCoords) * v_Color;
}