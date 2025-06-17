#version 300 es

precision mediump float;

layout(location = 0) out vec4 out_color;

in vec3 v_position;
in vec3 v_color;

uniform float u_time;
uniform vec3 u_color;

void main()
{
	out_color = vec4(u_color, 1);
}
