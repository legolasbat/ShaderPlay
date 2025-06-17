#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 v_position;
in vec3 v_color;

uniform vec3 u_resolution;
uniform float u_time;
uniform vec3 u_color;

void main()
{
	vec2 uv = v_position.xy / u_resolution.xy;

	vec3 color = 0.5 + 0.5 * cos(u_time + uv.xyx + vec3(0, 2, 4));

	out_color = vec4(color, 1);
}
