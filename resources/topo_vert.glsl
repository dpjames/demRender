#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec2 vTexCoord;
out vec3 fragNor;
out vec3 vpos;

void main()
{
	gl_Position = P * V * M * vertPos;
	fragNor = (V * M * vec4(vertNor, 0.0)).xyz;
   vpos = (V * M * vertPos).xyz;
   vTexCoord = vertTex;
}
