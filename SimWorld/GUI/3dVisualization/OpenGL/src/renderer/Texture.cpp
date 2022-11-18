#include "Texture.h"
#include "Renderer.h"
#include "GL/glew.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	:m_RendererID(0), m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	//flip the image up and down, since (0, 0) is at *left bottom* in the OpenGL texture coordinate.
	stbi_set_flip_vertically_on_load(1);
	
	/*
	*	Parameters of "stbi_load"
	* 
	*	const char* filename : path to the image
	*	int* x : store the width of the image
	*	int* y : store the height of the image
	*	int* channels_in_file: store the bits per pixel in the original image
	*	int desired_channels: the number of commponents excepted in the output stream, eg. RGBA is 4
	*/
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//specified sampled mode of the the texture
	
	//smaller than real pixels
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//largere than real pixels
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//??
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
