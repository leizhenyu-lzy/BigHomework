#pragma once

#include<string>
#include<unordered_map>

#include "glm/glm.hpp"

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	mutable std::unordered_map<std::string, int> m_LocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	//set Uniform
	void SetUniform1i(const std::string& name, int value);

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	void SetUniformArrayi(const std::string& name, const int* values, unsigned int count);

private:
	ShaderSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name) const;
};