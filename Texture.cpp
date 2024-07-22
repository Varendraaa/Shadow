#include "Texture.h"

GLuint TextureLoader::LoadTexture(const string& path)
{
	/*Texture Loading using STB*/
	int width, height, numColChannels;
	stbi_set_flip_vertically_on_load(true);		//Orients image properly since stb reads images differently to OpenGL

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numColChannels, 0);

	GLuint textureID{};
	
	/*Check for texture Format*/
	if (data)
	{
		GLenum format{};

		if (numColChannels == 1)
			format = GL_RED;
		else if (numColChannels == 3)
			format = GL_RGB;
		else if (numColChannels == 4)
			format = GL_RGBA;


		/*Creating the texture object*/
		
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);				//Activate Texture
		glBindTexture(GL_TEXTURE_2D, textureID);	//Bind texture to texture object

		/*Adjust Texture Parameters*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/*Generate Texture*/
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		cout << "Loaded texture " << path << " with dimensions: " << width << "x" << height << " and " << numColChannels << " components." << std::endl;

		glBindTexture(GL_TEXTURE_2D, 0);
		
		/*Delete Texture Image Data*/
		stbi_image_free(data);	
		
	}
	else
	{
		cerr << "Failed to load texture: " << path << endl;
		stbi_image_free(data);
	}

	return textureID;
}