#pragma once

#include "freeglut.h"
#include "Shader.h"
#include "MathHelper.h"

class Skybox {
public:
	GLuint vao, vio;
	GLuint textureID;
	GLuint mvpMatrixLoc, vertexLoc, textUnitLoc;
	Shader * shader;
	int numVertices, numIndices;
	GLuint buffers[2];

	~Skybox() {
		shader = 0;
		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &vao);
	}

	Skybox(Shader* _shader, GLuint imageId){
		shader = _shader;
		textureID = imageId;

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
		
		GLushort indices[num_indices] { 0, 2, 1, 1, 2, 3,   //front face
			4, 6, 5, 5, 6, 7,   //back face
			8, 10, 9, 9, 10, 11,   //right face
			12, 14, 13, 13, 14, 15,   //left face
			16, 18, 17, 17, 18, 19,   //bottom face
			20, 22, 21, 21, 22, 23 }; //top face
	
		init_geometry(vertices, indices);
	}

	void init_geometry(Vertex* vertices, GLushort* indices) {

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Generate slots for the vertex and color buffers
		glGenBuffers(2, buffers);

		// bind buffer for vertices and copy data into buffer
		vertexLoc = glGetAttribLocation(shader->id(), "position");
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(vertexLoc);
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

		textUnitLoc = glGetUniformLocation(shader->id(), "texCube");
		
		// bind buffer for indices and copy data into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLushort), indices, GL_STATIC_DRAW);
		vio = buffers[1];

		mvpMatrixLoc = glGetUniformLocation(shader->id(), "mvpMatrix");
	}
	
	void render_self(Matrix4 &self, Vector3 cameraPosition, float farPlane)
	{
		Matrix4 scale = Matrix4(farPlane / 1.73f, 0, 0, 0,
			0, farPlane / 1.73f, 0, 0,
			0, 0, farPlane / 1.73f, 0,
			0, 0, 0, 1
		);
		Matrix4 trans = Matrix4(1, 0, 0, cameraPosition.x,
			0, 1, 0, cameraPosition.y,
			0, 0, 1, cameraPosition.z,
			0, 0, 0, 1);

		shader->bind();
		glUniformMatrix4fv(mvpMatrixLoc, 1, true, (GLfloat*)&(self*trans*scale));
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glUniform1i(textUnitLoc, 0);

		glDrawElements(
			GL_TRIANGLES,            // mode
			numIndices,		     // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0             // element array buffer offset
			);
	}
};