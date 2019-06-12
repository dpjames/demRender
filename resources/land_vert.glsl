#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
layout(location = 3) in vec3 translation;
layout(location = 4) in vec3 scale;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec2 vTexCoord;
out vec3 fragNor;
out vec3 vpos;
in int gl_InstanceID;

void main()
{
   mat4 transmat = mat4(1.0);
   transmat[3][0] = translation[0];
   transmat[3][1] = translation[1];
   transmat[3][2] = translation[2];
   mat4 scalemat = mat4(1.0);
   scalemat[0][0] = scale[0];
   scalemat[1][1] = scale[1];
   scalemat[2][2] = scale[2];
   mat4 myM = M *  transmat * scalemat;
	gl_Position = P * V * myM * vertPos;
	fragNor = (V * myM *  vec4(vertNor, 0.0)).xyz;
   vpos = (V * myM * vertPos).xyz;
   vTexCoord = vertTex;
}
