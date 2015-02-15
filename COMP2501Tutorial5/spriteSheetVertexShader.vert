#version 140

uniform mat4 mvpMatrix;
uniform float numSpriteFrames;
uniform float currentFrame;

attribute vec3 position;
attribute vec2 textCoord;
 
varying vec2 TextCoord;


void main()
{
    gl_Position = mvpMatrix * vec4(position,1);
	TextCoord = vec2(textCoord.x/numSpriteFrames + currentFrame/numSpriteFrames,textCoord.y);
}
