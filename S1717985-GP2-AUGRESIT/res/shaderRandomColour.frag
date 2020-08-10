//Version number
#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;
//Unfrom variable
uniform float r;
uniform float g;
uniform float b;
 
void main()
{
//Setting each vector component to uniform varaible then setting final colour
	vec4 color;
	color = vec4(r,g,b,1.0);
    fragcolor = color;
}