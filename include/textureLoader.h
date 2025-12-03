#pragma once

#include "texture.h"

const int numberDefaultTextures = 22;

class TextureLoader {
protected:
	Texture* defaultTextures[numberDefaultTextures];

public:
	void LoadDefaultTextures();

	Texture* GetTextures(int index) const;
};
