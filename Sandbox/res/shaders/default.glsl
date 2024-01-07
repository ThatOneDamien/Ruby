#shader vertex
#version 450 core

layout(location = 0) in vec2 a_Position;

uniform mat4 u_ViewProj;

void main()
{
	gl_Position = u_ViewProj * vec4(a_Position.xy, 0.0, 1.0);
}

#shader fragment
#version 450 core

void main()
{
	gl_FragColor = vec4(0.2, 0.3, 0.8, 1.0);
}