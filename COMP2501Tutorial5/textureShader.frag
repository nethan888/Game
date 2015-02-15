#version 140

uniform sampler2D texUnit;

varying vec2 TextCoord;


void main() {
	gl_FragColor = texture(texUnit, TextCoord);
}
