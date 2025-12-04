#include "textureLoader.h"

#include <filesystem>

void TextureLoader::LoadDefaultTextures() {
	int i = 0;
	for (const auto& file : std::filesystem::directory_iterator(RESOURCES_PATH "textures")) {
		defaultTextures[i] = new Texture(file.path().string());
		defaultTextures[i]->LoadTexture();
		i++;
		if (i == numberDefaultTextures) {
			break;
		}
	}
}

Texture* TextureLoader::GetTextures(int index) const {
	return defaultTextures[index];
}
