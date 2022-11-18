#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec4 v_Color;

void main()
{
	gl_Position = position;
	v_Color = vec4(color, 1.0);
}

#shader fragment
#version 330 core

in vec4 v_Color;

layout(location = 0) out vec4 color;

void main()
{
	color = v_Color;
}