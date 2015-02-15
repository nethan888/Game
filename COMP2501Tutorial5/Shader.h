//Taken from: http://www.swiftless.com/
#ifndef __SHADER_H
#define __SHADER_H

#include <string>

#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <glew.h>
#include <GL/gl.h>
#include <glut.h>
#endif

class Shader {
public:
	Shader();
	Shader(const char *vsFile, const char *fsFile);
	~Shader();

	void init(const char *vsFile, const char *fsFile);

	void bind();
	void unbind();

	unsigned int id();

private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
};

#endif