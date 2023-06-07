#shader vertex
#version 410 core

layout(location = 0) in vec2 position;

void main()
{
	gl_Position = vec4(position.xy, 0.0, 1.0);
}

#shader fragment
#version 410 core

void main()
{
	gl_FragColor = vec4(0.3, 0.1, 0.1, 1.0);
}