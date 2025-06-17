char fragShaderText[2048] = 
"#version 460 core\n"
"layout(location = 0) out vec4 out_color;\n"

"in vec3 v_position;\n"
"in vec3 v_color;\n"

"uniform vec3 u_resolution;\n"
"uniform float u_time;\n"
"uniform vec3 u_color;\n"

"void main(){\n"
"	vec2 uv = v_position.xy / u_resolution.xy;\n"

"	vec3 color = 0.5 + 0.5 * cos(u_time + uv.xyx + vec3(0, 2, 4));\n"

"	out_color = vec4(color, 1);\n"
"}\n"
;

char vertShaderText[2048] =
"#version 460 core\n"

"layout(location = 0) in vec3 in_position;\n"
"layout(location = 1) in vec3 in_color;\n"

"out vec3 v_position;\n"
"out vec3 v_color;\n"

"void main(){\n"
"	gl_Position = vec4(in_position, 1);\n"
"	v_position = in_position;\n"
"	v_color = in_color;\n"
"}\n"
;
