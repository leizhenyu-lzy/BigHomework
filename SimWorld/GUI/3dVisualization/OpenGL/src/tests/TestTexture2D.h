#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include <memory>

namespace test {

	class TestTexture2D : public Test
	{
	private:
		std::shared_ptr<VertexBuffer> m_VB;
		std::shared_ptr<VertexArray> m_VAO;
		std::shared_ptr<IndexBuffer> m_IB;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Texture> m_Texture_1;
		std::shared_ptr<Texture> m_Texture_2;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation_A;
		glm::vec2 m_Position;
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}