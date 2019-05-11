#version 330 core 
uniform vec3 MatDif;
uniform vec3 lightCol;
uniform vec3 MatAmb;
uniform vec3 MatSpec;
uniform float shine;
uniform vec3 lightPos;
in vec3 fragNor;
in vec3 vpos;
out vec3 color;


void main()
{
	vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	vec3 Ncolor = 0.5*normal + 0.5;
	//color = vec4(Ncolor, 1.0);
	//color = vec3(theta,theta,theta);

   float cmag = length(lightCol);
   vec3 lightVec = normalize(lightPos - vpos);
   float costheta = max(0, dot(normal, lightVec));
   vec3 viewVec = normalize(-1 * vpos);
   vec3 H = normalize((lightVec + viewVec) / length(lightVec + viewVec));
   float red =   (MatSpec[0] * (pow(max(0,dot(normal,H)),shine)) * lightCol[0]) + MatAmb[0] + (costheta * cmag * MatDif[0] * lightCol[0]);
   float green = (MatSpec[1] * (pow(max(0,dot(normal,H)),shine)) * lightCol[1]) + MatAmb[1] + (costheta * cmag * MatDif[1] * lightCol[1]);
   float blue =  (MatSpec[2] * (pow(max(0,dot(normal,H)),shine)) * lightCol[2]) + MatAmb[2] + (costheta * cmag * MatDif[2] * lightCol[2]);
   color = vec3(red,green,blue);
}
