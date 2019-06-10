#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 V;
uniform float[320] m;
out vec2 vTexCoord;
out vec3 fragNor;
out vec3 vpos;
in int gl_InstanceID;

void main()
{
   int offset = gl_InstanceID * 16;
   mat4 M = mat4(
      vec4(m[offset + 0],m[offset + 1],m[offset + 2],m[offset + 3]),
      vec4(m[offset + 4],m[offset + 5],m[offset + 6],m[offset + 7]),
      vec4(m[offset + 8],m[offset + 9],m[offset + 10],m[offset + 11]),
      vec4(m[offset + 12],m[offset + 13],m[offset + 14],m[offset + 15])
   );
	gl_Position = P * V * M * vertPos;
	fragNor = (V * M *  vec4(vertNor, 0.0)).xyz;
   vpos = (V * M * vertPos).xyz;
   vTexCoord = vertTex;
}
