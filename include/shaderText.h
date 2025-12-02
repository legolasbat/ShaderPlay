const char fragShaderHeader[] =
"#version 460 core\n"
"layout(location = 0) out vec4 out_color;\n"

"in vec3 v_position;\n"

"uniform vec3      iResolution;           // viewport resolution (in pixels)\n"
"uniform float     iTime;                 // shader playback time (in seconds)\n"
"//uniform float     iTimeDelta;            // render time (in seconds)\n"
"//uniform float     iFrameRate;            // shader frame rate\n"
"uniform int       iFrame;                // shader playback frame\n"
"//uniform float     iChannelTime[4];       // channel playback time (in seconds)\n"
"//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)\n"
"uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click\n"
"//uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube\n"
"//uniform vec4      iDate;                 // (year, month, day, time in seconds)\n"
"//uniform float     iSampleRate;           // sound sample rate (i.e., 44100)\n"
;

const char fragShaderMain[] =
"void main(){\n"
"	mainImage(out_color, gl_FragCoord.xy);\n"
"}\n"
;

char fragShaderBody[64000] =
"void mainImage(out vec4 fragColor, in vec2 fragCoord){\n"
"	vec2 uv = fragCoord / iResolution.xy;\n"

"	vec3 color = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0, 2, 4));\n"

"	fragColor = vec4(color, 1);\n"
"}\n"
;

char vertShaderText[2048] =
"#version 460 core\n"

"layout(location = 0) in vec3 in_position;\n"

"out vec3 v_position;\n"

"void main(){\n"
"	gl_Position = vec4(in_position, 1);\n"
"	v_position = in_position;\n"
"}\n"
;
