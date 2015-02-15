#pragma once

#include "freeglut.h"
#include "Shader.h"
#include "MathHelper.h"
#include <glew.h>
#include <SOIL.h>
#include <unordered_map>
#include "MathHelper.h"


class Player {
public:
	Vector3 position;
	GLuint vao, vio;
	GLuint textureID;
	GLuint mvpMatrixLoc, vertexLoc, textUnitLoc, colorLoc, tCoordLoc;
	GLuint buffers[3];
	float velocity = 250;
	float accel = -0.28;
	bool jump = false;
	float landHeight = 0.0;

	Shader * shader;
	int numVertices, numIndices;

	~Player() {
		shader = 0;
		glDeleteBuffers(3, buffers);
		glDeleteVertexArrays(1, &vao);
	}

	Player(Shader* _shader, GLuint imageId){

		/*
		cube
		v7------v6
		/|      /|
		v2------v3|
		| |     | |
		| v5----|-v4
		|/      |/
		v0------v1

		*/
		shader = _shader;
		textureID = imageId;


		init_geometry();
	}
	void jumpplayer(float deltaTime){
		if (position.y < 1 && velocity < 0){
			jump = false;
			velocity = 250;
			accel = -0.28;
			position.y = landHeight;

		}
		else {
			velocity = velocity + accel*deltaTime;
			position.y += velocity / (deltaTime * 40);


			accel -= 0.017;

		}
	}


	void init_geometry() {

		const int num_vertices = 24;
		const int num_indices = 36;
		numVertices = num_vertices;
		numIndices = num_indices;


		Vertex vertices[num_vertices] = { Vertex(-1, -1, 1), Vertex(1, -1, 1), Vertex(-1, 1, 1), Vertex(1, 1, 1),  //front face  v0,v1,v2,v3
			Vertex(1, -1, -1), Vertex(-1, -1, -1), Vertex(1, 1, -1), Vertex(-1, 1, -1),   //back face   v4,v5,v6,v7
			Vertex(1, -1, 1), Vertex(1, -1, -1), Vertex(1, 1, 1), Vertex(1, 1, -1),   //right face  v1 v4 v3 v6
			Vertex(-1, -1, -1), Vertex(-1, -1, 1), Vertex(-1, 1, -1), Vertex(-1, 1, 1),   //left face   v5 v0 v7 v2
			Vertex(-1, -1, -1), Vertex(1, -1, -1), Vertex(-1, -1, 1), Vertex(1, -1, 1),  //bottom face v5 v4 v0 v1
			Vertex(-1, 1, 1), Vertex(1, 1, 1), Vertex(-1, 1, -1), Vertex(1, 1, -1) }; //top face    v2 v3 v7 v6

		Color colors[num_vertices] = { Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 0, 0, 1), //red
			Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 1, 0, 1),  //green
			Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(0, 0, 1, 1),  //blue
			Color(1, 1, 0, 1), Color(1, 1, 0, 1), Color(1, 1, 0, 1), Color(1, 1, 0, 1),  //yellow
			Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 0, 1, 1),  //magenta
			Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(0, 1, 1, 1) }; //cyan


		Texture2D textCoords[num_vertices] = { Texture2D(1.0f / 4.0f, 2.0f / 3.0f), Texture2D(2.0f / 4.0f, 2.0f / 3.0f), Texture2D(1.0f / 4.0f, 1.0f / 3.0f), Texture2D(2.0f / 4.0f, 1.0f / 3.0f),
			Texture2D(3.0f / 4.0f, 2.0f / 3.0f), Texture2D(1.0f, 2.0f / 3.0f), Texture2D(3.0f / 4.0f, 1.0f / 3.0f), Texture2D(1.0f, 1.0f / 3.0f),
			Texture2D(2.0f / 4.0f, 2.0f / 3.0f), Texture2D(3.0f / 4.0f, 2.0f / 3.0f), Texture2D(2.0f / 4.0f, 1.0f / 3.0f), Texture2D(3.0f / 4.0f, 1.0f / 3.0f),
			Texture2D(0.0f, 2.0f / 3.0f), Texture2D(1.0f / 4.0f, 2.0f / 3.0f), Texture2D(0.0f, 1.0f / 3.0f), Texture2D(1.0f / 4.0f, 1.0f / 3.0f),
			Texture2D(1.0f / 4.0f, 1.0f), Texture2D(2.0f / 4.0f, 1.0f), Texture2D(1.0f / 4.0f, 2.0f / 3.0f), Texture2D(2.0f / 4.0f, 2.0f / 3.0f),
			Texture2D(1.0f / 4.0f, 1.0f / 3.0f), Texture2D(2.0f / 4.0f, 1.0f / 3.0f), Texture2D(1.0f / 4.0f, 0.0f), Texture2D(2.0f / 4.0f, 0.0f) };


		GLushort indices[num_indices] = { 0, 1, 2, 2, 1, 3,   //front face
			4, 5, 6, 6, 5, 7,   //back face
			8, 9, 10, 10, 9, 11,   //right face
			12, 13, 14, 14, 13, 15,   //left face
			16, 17, 18, 18, 17, 19,   //bottom face
			20, 21, 22, 22, 21, 23 }; //top face


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

	}

	void render_self(Matrix4 &self)
	{


		shader->bind();

		glUniformMatrix4fv(mvpMatrixLoc, 1, true, (GLfloat*)&self);

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