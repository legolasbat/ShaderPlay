#version 300 es

precision highp float;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 v_position;
out vec3 v_color;

void main()
{
	gl_Position = vec4(in_position, 1);
	v_position = in_position;
	v_color = in_color;
}
