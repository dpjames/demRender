#version 330 core 
in vec3 fragNor;
in vec3 vpos;
out vec3 color;
void main()
{
   vec3 MatDif = vec3(.5,.5,.5);
   vec3 lightColor = vec3(1,1,1);
   vec3 MatAmb = vec3(.2,.2,.2);
   vec3 MatSpec = vec3(.01,.01,.01);
   float shine = .01;
   vec3 lightPos = vec3(0,0,30);


	vec3 normal = normalize(fragNor);
	vec3 Ncolor = 0.5*normal + 0.5;
   float cmag = length(lightColor);
   vec3 lightVec = normalize(lightPos - vpos);
   float costheta = max(0, dot(normal, lightVec));
   vec3 viewVec = normalize(-1 * vpos);
   vec3 H = normalize((lightVec + viewVec) / length(lightVec + viewVec));
   float red =   (MatSpec[0] * (pow(max(0,dot(normal,H)),shine)) * lightColor[0]) + MatAmb[0] + (costheta * cmag * MatDif[0] * lightColor[0]);
   float green = (MatSpec[1] * (pow(max(0,dot(normal,H)),shine)) * lightColor[1]) + MatAmb[1] + (costheta * cmag * MatDif[1] * lightColor[1]);
   float blue =  (MatSpec[2] * (pow(max(0,dot(normal,H)),shine)) * lightColor[2]) + MatAmb[2] + (costheta * cmag * MatDif[2] * lightColor[2]);
   color = vec3(red,green,blue);
}
