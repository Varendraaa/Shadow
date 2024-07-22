//The TextureLoader class will be responsible for loading textures from files and creating OpenGL texture objects.
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>
#include <iostream>

using namespace std;

class TextureLoader
{
public:
	static GLuint LoadTexture(const string& path);
};

#endif // !TEXTURELOADER_H

