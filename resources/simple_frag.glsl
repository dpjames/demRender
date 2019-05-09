#version 330 core 
out vec4 color;
in vec3 vcol;
void main()
{
	// vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	// vec3 Ncolor = 0.5*normal + 0.5;
   color = vec4(vcol[0],vcol[1],vcol[2],1);
}
