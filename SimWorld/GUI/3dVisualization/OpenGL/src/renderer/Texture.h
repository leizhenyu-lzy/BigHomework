#pragma once

#include<string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetBitPerPixrl() const { return m_BPP; }
	inline unsigned int GetRendererID() const { return m_RendererID; }
};