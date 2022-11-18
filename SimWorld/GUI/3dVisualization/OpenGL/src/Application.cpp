#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestCube.h"
#include "tests/TestGridmap.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 960, "Traffic Visualization System", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	{
		/* Setup Dear ImGui context */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		

		/* Setup Dear ImGui style */
		ImGui::StyleColorsDark();

		/* Setup Platform / Renderer bindings */ 
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		Renderer renderer;

		test::TestMenu* testMenu = new test::TestMenu(window);
		//testMenu->ResisterTest<test::TestCube>("Draw Cube");
		testMenu->ResisterTest<test::TestGridmap>("Draw Gridmap");
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (testMenu->GetCurrentTest())
			{
				testMenu->GetCurrentTest()->OnUpdate(50.0f);
				testMenu->GetCurrentTest()->OnRender();

				ImGui::Begin("Menu bar");
				ImGui::SetWindowFontScale(2.0f);

				if (testMenu->GetCurrentTest() != testMenu && ImGui::Button("<-"))
				{
					testMenu->Return();
				}

				testMenu->GetCurrentTest()->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		/* delete test menu */
		delete testMenu;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}