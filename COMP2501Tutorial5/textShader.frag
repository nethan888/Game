#version 150
 
varying vec2 TextCoord; 
uniform sampler2D texUnit;
uniform vec4 textColor;


void main()
{
	vec4 temp = texture(texUnit, TextCoord);
	if (temp.w < 0.1) discard; //discard pixels with low alpha value
	//Note this could also be used to set a background color
	gl_FragColor = textColor;
}