#pragma once

#include "freeglut.h"
#include "Shader.h"
#include "MathHelper.h"

class Quad {
public:
	GLuint vao, vio;
	GLuint textureID;
	GLuint mvpMatrixLoc, vertexLoc, textUnitLoc, colorLoc, tCoordLoc;
	GLuint numSpriteFramesLoc, currentFrameLoc;
	int numSpriteFrames = 1; //default hard code
	int currentFrame = 0; //which frame

	GLuint buffers[3];

	Shader * shader; //should be a sprite sheet shader
	int numVertices, numIndices;

	~Quad() {
		shader = 0;
		glDeleteBuffers(3, buffers);
		glDeleteVertexArrays(1, &vao);
	}

	Quad(Shader* _shader, GLuint imageId, int aNumberOfSpriteStripFrames){

		/*
		quad
		v2------v3
		|       | 
		|       |
		|       |
		v0------v1

		*/
		shader = _shader;
		textureID = imageId;
		numSpriteFrames = aNumberOfSpriteStripFrames;

		init_geometry();
	}

	void setCurrentFrame(int frameNumber) { currentFrame = frameNumber; }
	void advanceFrame() { currentFrame = (currentFrame + 1) % numSpriteFrames; }

	void init_geometry() {

		const int num_vertices = 4;
		const int num_indices = 6;
		numVertices = num_vertices;
		numIndices = num_indices;


		Vertex vertices[num_vertices] = { 
			Vertex(-1, -1, 1), Vertex(1, -1, 1), Vertex(-1, 1, 1), Vertex(1, 1, 1) //face  v0,v1,v2,v3
			}; 

		Color colors[num_vertices] = { 
			Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 0, 0, 1) //red
			}; 


		Texture2D textCoords[num_vertices] = { 
			Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f)
			};


		GLushort indices[num_indices] = { 
			0, 1, 2, 2, 1, 3   //front face
			}; //top face


		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Generate slots for the vertex and color buffers
		glGenBuffers(3, buffers);


		// bind buffer for vertices and copy data into buffer
		vertexLoc = glGetAttribLocation(shader->id(), "position");
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(vertexLoc);
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);


		if (textureID)
		{
			tCoordLoc = glGetAttribLocation(shader->id(), "textCoord");
			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Texture2D), textCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(tCoordLoc);
			glVertexAttribPointer(tCoordLoc, 2, GL_FLOAT, 0, 0, 0);

			// Typical Texture Generation Using Data From The Bitmap

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			textUnitLoc = glGetUniformLocation(shader->id(), "texUnit");
		}

		else {
			colorLoc = glGetAttribLocation(shader->id(), "color");
			glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
			glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Color), colors, GL_STATIC_DRAW);
			glEnableVertexAttribArray(colorLoc);
			glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
		}

		// bind buffer for indices and copy data into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLushort), indices, GL_STATIC_DRAW);
		vio = buffers[2];

		mvpMatrixLoc = glGetUniformLocation(shader->id(), "mvpMatrix");

		numSpriteFramesLoc = glGetUniformLocation(shader->id(), "numSpriteFrames");
		currentFrameLoc = glGetUniformLocation(shader->id(), "currentFrame");

	}

	void render_self(Matrix4 &self)
	{

		shader->bind();

		glUniformMatrix4fv(mvpMatrixLoc, 1, true, (GLfloat*)&self);

		//set current frame and number of frames for shader
		glUniform1f(numSpriteFramesLoc, (float)numSpriteFrames);
		glUniform1f(currentFrameLoc, (float)currentFrame);

		if (textureID){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glUniform1i(textUnitLoc, 0);
		}
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);

		glDrawElements(
			GL_TRIANGLES,        // mode
			numIndices,			 // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0             // element array buffer offset
			);

	}
};