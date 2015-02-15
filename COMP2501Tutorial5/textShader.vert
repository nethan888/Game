#version 140
 
attribute vec4 position;
attribute vec2 textCoord;

varying vec2 TextCoord;
uniform mat4 mvpMatrix;
uniform float letter;
 
void main()
{
    gl_Position = mvpMatrix * position;
    TextCoord = vec2(textCoord.x + letter,textCoord.y);
}