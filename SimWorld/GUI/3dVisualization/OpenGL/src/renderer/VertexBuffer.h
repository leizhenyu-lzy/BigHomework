#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void SetData(int offset, unsigned int size, const void* data);
	void Bind() const;
	void UnBind() const;
};