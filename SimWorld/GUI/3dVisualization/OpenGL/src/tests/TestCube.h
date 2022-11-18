#pragma once

#include "Test.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "helper/trackball.h"
#include "helper/glslprogram.h"
#include "helper/vboplane.h"
#include "helper/vbocube.h"
#include "helper/vbotorus.h"

#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <functional>

namespace test {
	class TestCube : public Test
	{
	private:
		// The skybox cube model.
		Drawable* skyboxModel;

		// 3D object model (brick cube).
		Drawable* obj3DModel1;

		// 3D object model (wooden cube).
		Drawable* obj3DModel2;
		
		// Shaders' filenames.
		constexpr static const char vertShaderFile[] = "res/shaders/shader.vert";
		constexpr static const char fragShaderFile[] = "res/shaders/shader.frag";

		GLSLProgram shaderProg;  // Contains the shader program object.

		// Light info.
		static GLfloat initial_lightPosition[4];  // Directional light. Assumed in eye space.
		static GLfloat lightAmbient[4];
		static GLfloat lightDiffuse[4];
		static GLfloat lightSpecular[4];
		static GLfloat lightPosition[4];  // Directional light. Assumed in eye space.

		static float lightPosIncr; // Step size to move light position in x & y directions.

		static float skyboxSize;  // Desired width of the skybox.


		// For window and viewport size.
		static int winWidth;     // Window width in pixels.
		static int winHeight;    // Window height in pixels.

		static bool wireframeMode ;

		// For trackball.
		static double prevMouseX, prevMouseY;
		static bool mouseLeftPressed;
		static bool mouseMiddlePressed;
		static bool mouseRightPressed;
		static float cam_curr_quat[4];
		static float cam_prev_quat[4];
		static float cam_eye[3], cam_lookat[3], cam_up[3];
		static float initial_cam_pos[3];  // World coordinates.
		
		
	public:
		TestCube();
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void SetupEnvMap(void);
		void SetupTextureMaps(void);

		// TODO:move the function to update funtion
		friend void MyKeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);

	};


}