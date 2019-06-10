#version 330 core 
uniform sampler2D Texture0;
uniform vec3 MatDif;
uniform vec3 lightCol;
uniform vec3 MatAmb;
uniform vec3 MatSpec;
uniform float shine;
uniform vec3 lightPos;
in vec3 fragNor;
in vec3 vpos;
in vec2 vTexCoord;
out vec4 color;


void main()
{
   //vec4 texColor0 = texture(Texture0, vTexCoord);
	vec3 normal = normalize(fragNor);
   float cmag = length(lightCol);
   vec3 lightVec = normalize(lightPos - vpos);
   float costheta = max(0, dot(normal, lightVec));
   vec3 viewVec = normalize(-1 * vpos);
   vec3 H = normalize((lightVec + viewVec) / length(lightVec + viewVec));
   float red =   ((MatSpec[0] * (pow(max(0,dot(normal,H)),shine)) * lightCol[0]) + MatAmb[0] + (costheta * cmag * MatDif[0] * lightCol[0])); //* texColor0[0];
   float green = ((MatSpec[1] * (pow(max(0,dot(normal,H)),shine)) * lightCol[1]) + MatAmb[1] + (costheta * cmag * MatDif[1] * lightCol[1])); //* texColor0[1];
   float blue =  ((MatSpec[2] * (pow(max(0,dot(normal,H)),shine)) * lightCol[2]) + MatAmb[2] + (costheta * cmag * MatDif[2] * lightCol[2])); //* texColor0[2];
   //if(texColor0[0] > .9 && texColor0[1] > .9 && texColor0[2] > .9){
   //   discard;
   //} else {
   //color = vec4(red,green,blue,1);
   //}
   color = vec4(red,green,blue,1);
   //color = vec4(1,0,0,1);
}
