#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
layout(location = 3) in vec4 m1;
layout(location = 4) in vec4 m2;
layout(location = 5) in vec4 m3;
layout(location = 6) in vec4 m4;
uniform mat4 P;
uniform mat4 V;
out vec2 vTexCoord;
out vec3 fragNor;
out vec3 vpos;
in int gl_InstanceID;

void main()
{
   mat4 M = mat4(m1,m2,m3,m4);
	gl_Position = P * V * M * vertPos;
	fragNor = (V * M *  vec4(vertNor, 0.0)).xyz;
   vpos = (V * M * vertPos).xyz;
   vTexCoord = vertTex;
}
