#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Test.h"
#include "imgui/imgui.h"

namespace test{

	void Test::SetWindow(GLFWwindow* window)
	{
		m_Window = window;
	}

	GLFWwindow* Test::GetWindow()
	{
		return m_Window;
	}

	TestMenu::TestMenu(GLFWwindow* window)
	{
		m_Window = window;
		m_CurrentTest = this;
	}
	TestMenu::~TestMenu()
	{
		if (m_CurrentTest != this)
		{
			delete m_CurrentTest;
		}
	}
	void TestMenu::Return()
	{
		if (m_CurrentTest != this)
		{
			delete m_CurrentTest;
			m_CurrentTest = this;
		}
	}
	void TestMenu::OnImGuiRender()
	{
		for (auto& test: m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
				// Tranfer window.
				m_CurrentTest->SetWindow(m_Window);
			}
		}
	}
}