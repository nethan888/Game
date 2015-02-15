#include "MathHelper.h"

#define IMG_RATIO 1.0f/128.0f

class Text
{
public:
	Text(Shader* _shader, GLuint textId, float lWidth, float lHeight) { 
		shader = _shader;
		textureId = textId;
		letterWidth = lWidth;
		letterHeight = lHeight;
		initGeometry(); 
	}

	~Text()
	{
		shader = 0;
		glDeleteBuffers(5, buffers);
		glDeleteVertexArrays(1, &vao);
	}

	void initGeometry()
	{
		GLuint vertexLoc, normalLoc, tCoordLoc;

		Vertex vertices[4] = { Vertex(-0.5, -0.5, 0), Vertex(0.5, -0.5, 0), Vertex(-0.5, 0.5, 0), Vertex(0.5, 0.5, 0) };
		GLushort indices[6] = { 0, 1, 2, 2, 1, 3 };
		Texture2D tCoords[4] = { Texture2D(0, 1), Texture2D(IMG_RATIO, 1), Texture2D(0, 0), Texture2D(IMG_RATIO, 0) };
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Generate slots for the vertex and color buffers
		glGenBuffers(4, buffers);

		// bind buffer for vertices and copy data into buffer
		vertexLoc = glGetAttribLocation(shader->id(), "position");
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(vertexLoc);
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);

		tCoordLoc = glGetAttribLocation(shader->id(), "textCoord");
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Texture2D), &tCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(tCoordLoc);
		glVertexAttribPointer(tCoordLoc, 2, GL_FLOAT, 0, 0, 0);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// bind buffer for indices and copy data into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indices, GL_STATIC_DRAW);
		vio = buffers[4];

		textUnitLoc = glGetUniformLocation(shader->id(), "texUnit");
		mvpMatrixLoc = glGetUniformLocation(shader->id(), "mvpMatrix");
		colorLoc = glGetUniformLocation(shader->id(), "textColor");
		letterLoc = glGetUniformLocation(shader->id(), "letter");
	}
	void render(Matrix4 &parent, std::string word)
	{
		int numInstances = 0;
		Matrix4 scale = Matrix4::IDENTITY;
		Matrix4 translation = Matrix4::IDENTITY;
		Matrix4 mvpMatrix = Matrix4::IDENTITY;
		float letterPosition = 0;

		scale = scaleMatrix(Vector3(letterWidth, letterHeight, 1));
		translation = translationMatrix(position);
		mvpMatrix = parent * translation * scale;
	

		shader->bind();

		glBindVertexArray(vao);

		glUniformMatrix4fv(mvpMatrixLoc, 1, GL_TRUE, (GLfloat *)&mvpMatrix);
		glUniform1f(letterLoc, letterPosition);
		glUniform4fv(colorLoc, 1, (GLfloat *)&color);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(textUnitLoc, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,(void*)0);
		for (int i = 0; i < word.length(); i++){
			
			translation = translationMatrix(position + Vector3(i*letterWidth, 0, 0));
			mvpMatrix = parent * translation *scale;
			letterPosition = word[i] * IMG_RATIO;
			glUniformMatrix4fv(mvpMatrixLoc, 1, GL_TRUE, (GLfloat*)&mvpMatrix);
			glUniform1f(letterLoc, letterPosition);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
		}

	}

	void setLetterWidth(float lWidth){ letterWidth = lWidth;}
	void setLetterHeight(float lHeight){ letterHeight = lHeight; }
	void setText(std::string _text){ text=_text; }
	void setPosition(Vector3 pos){ position = pos; }
	void setWrapLength(int wrap) { wrapLength = wrap; }
	void setColor(Color col) { color = col; }
	void setLineSpacing(float space) { lineSpacing = space; }

private:
	Vector3 position = Vector3(-300,0,0);
	GLuint vao, vio;
	Shader* shader;
	GLuint mvpMatrixLoc, textUnitLoc;
	GLuint buffers[5];
	GLuint textureId, letterLoc, colorLoc;
	float letterHeight = 1.0f;
	float letterWidth = 1.0f;
	int wrapLength = -1;
	float lineSpacing = 1.0f;
	std::string text;
	Color color = Color(1,1,1,1);
};