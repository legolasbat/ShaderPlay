#pragma once

#include <glad/glad.h>

class Plane {
	
	static constexpr float planeVertices[12] = {
		1.0f, 1.0f, 0.0f,	// vertex 1
		-1.0f, 1.0f, 0.0f,	// vertex 2
		-1.0f, -1.0f, 0.0f,	// vertex 3
		1.0f, -1.0f, 0.0f	// vertex 4
	};

	static constexpr unsigned short indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO;

public:

	Plane() {};

	void init() {
		VAO = 0;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Create buffer
		GLuint buffer = 0;
		glGenBuffers(1, &buffer);

		// Bind buffer and send data
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		// Attribute 0: Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

		// Create buffer
		GLuint indexBuffer = 0;
		glGenBuffers(1, &indexBuffer);

		// Bind buffer and send data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		glDeleteBuffers(1, &buffer);
		glDeleteBuffers(1, &indexBuffer);
	}

	void Draw() const {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	}

	~Plane() {
		glDeleteVertexArrays(1, &VAO);
	}
};
