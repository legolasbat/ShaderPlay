#pragma once

#include <glad/glad.h>
#include <string>

enum Filter {
	nearest,
	linear,
	mipmap
};

enum Wrap {
	clamp,
	repeat
};

class Texture {
public:
	Texture();
	Texture(std::string fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	bool LoadTextureFlip();

	void GetTextureID(GLuint* textureID) const;
	void GetTextureSize(int* width, int* height) const;

	void UseTexture(int index) const;
	void ClearTexture();

	void SetFilter(Filter filter);
	Filter GetFilter() const;
	void SetWrap(Wrap wrap);
	Wrap GetWrap() const;

	bool GetFlip() const;

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	Filter filter;
	Wrap wrap;

	bool isFlip;

	std::string fileLocation;
};
