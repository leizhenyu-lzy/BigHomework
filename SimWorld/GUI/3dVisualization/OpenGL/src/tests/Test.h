#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace test{

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		
		void SetWindow(GLFWwindow* window);
		GLFWwindow* GetWindow();
		
	protected:
		GLFWwindow* m_Window;
	};

	class TestMenu: public Test
	{
	private:
		Test* m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()> > > m_Tests;
		
	public:
		TestMenu(GLFWwindow* window);
		~TestMenu();

		template<typename T>
		void ResisterTest(const std::string& name)
		{
			std::cout << "Register test: " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

		void Return();

		void OnImGuiRender() override;
		inline Test* GetCurrentTest() const { return m_CurrentTest; }
	};
}