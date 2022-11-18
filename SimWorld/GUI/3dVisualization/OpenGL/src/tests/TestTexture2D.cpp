#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include <array>
#include <string.h>

namespace test {
	struct Vec2
	{
		float x, y;
	};
	struct Vec3
	{
		float x, y, z;
	};
	struct Vec4
	{
		float x, y, z, w;
	};
	struct Vertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoords;
		float TexID;
	};

	static Vertex* CreateQuad(Vertex* target, float x, float y, float TexID)
	{
		float size = 200.0f;

		target->Position = { x, y+size, 0.0f };
		target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		target->TexCoords = { 0.0f, 1.0f };
		target->TexID = TexID;
		target++;

		target->Position = { x + size, y+size, 0.0f };
		target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		target->TexCoords = { 1.0f, 1.0f };
		target->TexID = TexID;
		target++;

		target->Position = { x + size, y, 0.0f };
		target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		target->TexCoords = { 1.0f, 0.0f };
		target->TexID = TexID;
		target++;

		target->Position = { x, y, 0.0f };
		target->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		target->TexCoords = { 0.0f, 0.0f };
		target->TexID = TexID;
		target++;

		return target;
	}

	TestTexture2D::TestTexture2D()
		:m_Proj(glm::ortho(0.0f, 1280.0f, 0.0f, 960.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Translation_A(0, 0, 0), m_Position(0, 0)
	{
		//TODO: abstract objects class

		//TODO: combine vertex buffer

		/*float vertex[] = {
			//first quard
			0.0f,   500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //0
			500.0f, 500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //1
			500.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //2
			0.0f,   0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //3
			//second quard
			550.0f,  500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //4
			1050.0f, 500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //5
			1050.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //6
			550.0f,  0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //7
		};*/

		const unsigned int MaxQuadCount = 1000;
		const unsigned int MaxVertexCount = MaxQuadCount * 4;
		const unsigned int MaxIndexCount = MaxQuadCount * 6;

		//TODO: combine index buffer
		//unsigned int indices[] = {
		//	//first quard
		//	1, 0, 3,
		//	3, 2, 1,
		//	//second quard
		//	5, 4, 7,
		//	7, 6, 5
		//};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_shared<VertexArray>();

		//assigned nullptr to data and allocate a maximum number of GPU memory
		m_VB = std::make_shared<VertexBuffer>(nullptr, sizeof(Vertex) * MaxVertexCount);

		VertexBufferLayout layout;
		layout.Push<float>(3);//positon
		layout.Push<float>(4);//vertex color
		layout.Push<float>(2);//texture coordinates
		layout.Push<float>(1);//texture slot

		m_VAO->AddBuffer(*m_VB, layout);

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		//m_IB = std::make_shared<IndexBuffer>(indices, sizeof(indices)/sizeof(uint32_t));
		m_IB = std::make_shared<IndexBuffer>(indices, 6*26);

		m_Shader = std::make_shared<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		m_Texture_1 = std::make_shared<Texture>("res/texture/texture_test.png");
		m_Texture_2 = std::make_shared<Texture>("res/texture/ChernoLogo.png");

		//Bind different textures
		GLCall(glBindTextureUnit(0, m_Texture_1->GetRendererID()));
		GLCall(glBindTextureUnit(1, m_Texture_2->GetRendererID()));

		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		int samplers[2] = { 0, 1 };

		m_Shader->SetUniformArrayi("u_Texture", samplers, 2);
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		//vertices Hard code
		/*float vertex[] = {
			//first quard
			0.0f,   500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //0
			500.0f, 500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //1
			500.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //2
			0.0f,   0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //3
			//second quard
			550.0f,  500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //4
			1050.0f, 500.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //5
			1050.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //6
			550.0f,  0.0f,   0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //7
		};*/
		
		std::array<Vertex, 1000> vertices;
		Vertex* buffer = vertices.data();

		for (int y = 0; y < 1000; y+=200)
		{
			for (int x = 0; x < 1000; x+=200)
			{
				buffer = CreateQuad(buffer, x, y, ((x+y)/200)%2);
			}
		}
		buffer = CreateQuad(buffer, m_Position.x, m_Position.y, 0.0f);

		/*auto q0 = CreateQuad(m_Position.x, m_Position.y, 0.0f);
		auto q1 = CreateQuad(m_Position.x + 550.0f, m_Position.y, 1.0f);

		Vertex vertices[8];
		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));*/

		//set dynamic vertex buffer
		m_VB->SetData(0, vertices.size() * sizeof(Vertex), vertices.data());

		GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Shader->Bind();

		//draw
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation_A);
			//projection * view * model
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IB, *m_Shader);
		}
	}
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation_A", &m_Translation_A.x, 0.0f, 600.0f);

		ImGui::DragFloat2("Control", &m_Position.x, 1.0f, 0.0f, 200.0f);
	}
}