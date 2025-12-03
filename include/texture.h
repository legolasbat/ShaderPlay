#pragma once

#include <glad/glad.h>

class Texture {
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void GetTextureID(GLuint* textureID) const;
	void GetTextureSize(int* width, int* height) const;

	void UseTexture(int index) const;
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};
