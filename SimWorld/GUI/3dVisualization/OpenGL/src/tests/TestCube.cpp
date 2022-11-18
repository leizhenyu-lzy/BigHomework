#include "TestCube.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include "stb_image/stb_image.h"

namespace test {
	// static variables initialization
    // Light info.
    GLfloat TestCube::initial_lightPosition[] = { 5.0f, 3.0f, 10.0f, 0.0f };  // Directional light. Assumed in eye space.
    GLfloat TestCube::lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat TestCube::lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat TestCube::lightSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat TestCube::lightPosition[4];  // Directional light. Assumed in eye space.

    float TestCube::lightPosIncr = 0.5f; // Step size to move light position in x & y directions.

    float TestCube::skyboxSize = 50.0f;  // Desired width of the skybox.

    // For window and viewport size.
    int TestCube::winWidth = 800;     // Window width in pixels.
    int TestCube::winHeight = 600;    // Window height in pixels.

    bool TestCube::wireframeMode;

    // For trackball.
    double TestCube::prevMouseX, TestCube::prevMouseY;
    bool TestCube::mouseLeftPressed;
    bool TestCube::mouseMiddlePressed;
    bool TestCube::mouseRightPressed;
    float TestCube::cam_curr_quat[4];
    float TestCube::cam_prev_quat[4];
    float TestCube::cam_eye[3], TestCube::cam_lookat[3], TestCube::cam_up[3];
    float TestCube::initial_cam_pos[3] = { 0.0, 0.0, 3.0f };  // World coordinates.
	


    // Not activated
    void MyKeyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {

            // Close window
            if (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            else if (key == GLFW_KEY_W) {
                TestCube::wireframeMode = !TestCube::wireframeMode;
            }

            else if (key == GLFW_KEY_R) {
                // Reset light position.
                TestCube::lightPosition[0] = TestCube::initial_lightPosition[0];
                TestCube::lightPosition[1] = TestCube::initial_lightPosition[1];
                TestCube::lightPosition[2] = TestCube::initial_lightPosition[2];
                TestCube::lightPosition[3] = TestCube::initial_lightPosition[3];

                // Reset the trackball.
                trackball(TestCube::cam_curr_quat, 0, 0, 0, 0);
                TestCube::cam_eye[0] = TestCube::initial_cam_pos[0];
                TestCube::cam_eye[1] = TestCube::initial_cam_pos[1];
                TestCube::cam_eye[2] = TestCube::initial_cam_pos[2];
                TestCube::cam_lookat[0] = 0.0f;
                TestCube::cam_lookat[1] = 0.0f;
                TestCube::cam_lookat[2] = 0.0f;
                TestCube::cam_up[0] = 0.0f;
                TestCube::cam_up[1] = 1.0f;
                TestCube::cam_up[2] = 0.0f;
            }

            // Move the light.
            else if (key == GLFW_KEY_UP) {
                TestCube::lightPosition[1] += TestCube::lightPosIncr;
            }
            else if (key == GLFW_KEY_DOWN) {
                TestCube::lightPosition[1] -= TestCube::lightPosIncr;
            }
            else if (key == GLFW_KEY_LEFT) {
                TestCube::lightPosition[0] -= TestCube::lightPosIncr;
            }
            else if (key == GLFW_KEY_RIGHT) {
                TestCube::lightPosition[0] += TestCube::lightPosIncr;
            }
        }
    }
	
	TestCube::TestCube()
	{
        // Install shaders.
        try {
            shaderProg.compileShader(vertShaderFile, GLSLShader::VERTEX);
            shaderProg.compileShader(fragShaderFile, GLSLShader::FRAGMENT);
            shaderProg.link();
            // shaderProg.validate();
            shaderProg.use(); // Install shader program to rendering pipeline.
        }
        catch (GLSLProgramException& e) {
            fprintf(stderr, "Error: %s.\n", e.what());
            exit(EXIT_FAILURE);
        }

        // Create geometry of 3D object models.
        skyboxModel = new VBOCube;
        obj3DModel1 = new VBOCube;
        obj3DModel2 = new VBOCube;

        // Set up environment cubemap.
        SetupEnvMap();

        // Set up texture maps.
        SetupTextureMaps();

        // Initialize some OpenGL states.
        glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to space black.
        glEnable(GL_DEPTH_TEST); // Use depth-buffer for hidden surface removal.

        // Initialize light position.
        lightPosition[0] = initial_lightPosition[0];
        lightPosition[1] = initial_lightPosition[1];
        lightPosition[2] = initial_lightPosition[2];
        lightPosition[3] = initial_lightPosition[3];

        // Initialization for trackball.
        trackball(cam_curr_quat, 0, 0, 0, 0);
        cam_eye[0] = initial_cam_pos[0];
        cam_eye[1] = initial_cam_pos[1];
        cam_eye[2] = initial_cam_pos[2];
        cam_lookat[0] = 0.0f;
        cam_lookat[1] = 0.0f;
        cam_lookat[2] = 0.0f;
        cam_up[0] = 0.0f;
        cam_up[1] = 1.0f;
        cam_up[2] = 0.0f;

		// Initialize callback functions.
        //m_MyReshapeFunc = MyReshapeFunc;
	}

	TestCube::~TestCube()
	{
        delete skyboxModel;
        delete obj3DModel1;
        delete obj3DModel2;
	}

	void TestCube::OnUpdate(float deltaTime)
	{

        // Reshape function
        {
			int w, h;
			glfwGetFramebufferSize(m_Window, &w, &h);
            winWidth = w;
            winHeight = h;
			glViewport(0, 0, w, h);
		}
		
		// Mouse motion
        {
            double mouse_x, mouse_y;
            glfwGetCursorPos(m_Window, &mouse_x, &mouse_y);
            float rotScale = 1.0f;
            float transScale = 2.0f;

            if (TestCube::mouseLeftPressed) {
                trackball(TestCube::cam_prev_quat, rotScale * (2.0f * TestCube::prevMouseX - TestCube::winWidth) / (float)TestCube::winWidth,
                    rotScale * (TestCube::winHeight - 2.0f * TestCube::prevMouseY) / (float)TestCube::winHeight,
                    rotScale * (2.0f * mouse_x - TestCube::winWidth) / (float)TestCube::winWidth,
                    rotScale * (TestCube::winHeight - 2.0f * mouse_y) / (float)TestCube::winHeight);

                add_quats(TestCube::cam_prev_quat, TestCube::cam_curr_quat, TestCube::cam_curr_quat);
            }
            else if (TestCube::mouseMiddlePressed) {
                TestCube::cam_eye[0] -= transScale * (mouse_x - TestCube::prevMouseX) / (float)TestCube::winWidth;
                TestCube::cam_lookat[0] -= transScale * (mouse_x - TestCube::prevMouseX) / (float)TestCube::winWidth;
                TestCube::cam_eye[1] += transScale * (mouse_y - TestCube::prevMouseY) / (float)TestCube::winHeight;
                TestCube::cam_lookat[1] += transScale * (mouse_y - TestCube::prevMouseY) / (float)TestCube::winHeight;
            }
            else if (TestCube::mouseRightPressed) {
                TestCube::cam_eye[2] += transScale * (mouse_y - TestCube::prevMouseY) / (float)TestCube::winHeight;
                TestCube::cam_lookat[2] += transScale * (mouse_y - TestCube::prevMouseY) / (float)TestCube::winHeight;
            }

            // Update mouse point
            TestCube::prevMouseX = mouse_x;
            TestCube::prevMouseY = mouse_y;
        }
		
        // Mouse Click
        {
            int action;
            if (action=glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT))
            {
                TestCube::mouseLeftPressed = true;
                trackball(TestCube::cam_prev_quat, 0.0, 0.0, 0.0, 0.0);
            }
            else if (action = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT))
            {
                TestCube::mouseRightPressed = true;
            }
            else if (action = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE))
            {
                TestCube::mouseMiddlePressed = true;
            }
            else
            {
                TestCube::mouseLeftPressed = false;
				TestCube::mouseMiddlePressed = false;
				TestCube::mouseRightPressed = false;
            }
        }
		

		
	}

	void TestCube::OnRender()
	{	
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //=======================================================================
        // Pass texture unit numbers of texture maps to shaders.
        //=======================================================================

        shaderProg.setUniform("EnvMap", 0);
        shaderProg.setUniform("BrickDiffuseMap", 1);
        shaderProg.setUniform("BrickNormalMap", 2);
        shaderProg.setUniform("WoodDiffuseMap", 3);

        //=======================================================================
        // Set up View and Projection matrices.
        //=======================================================================

        // Perspective projection matrix.
        glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float)winWidth / winHeight, 0.1f, 100.0f);

        // View transformation matrix.
        glm::mat4 viewMat = glm::lookAt(glm::vec3(cam_eye[0], cam_eye[1], cam_eye[2]),
            glm::vec3(cam_lookat[0], cam_lookat[1], cam_lookat[2]),
            glm::vec3(cam_up[0], cam_up[1], cam_up[2]));

        // Get the additional camera rotation matrix introduced by trackball.
        GLfloat rot[4][4];
        build_rotmatrix(rot, cam_curr_quat);
        glm::mat4 camRotMat = glm::mat4(rot[0][0], rot[0][1], rot[0][2], rot[0][3],
            rot[1][0], rot[1][1], rot[1][2], rot[1][3],
            rot[2][0], rot[2][1], rot[2][2], rot[2][3],
            rot[3][0], rot[3][1], rot[3][2], rot[3][3]);

        // The final view transformation has the additional rotation from trackball.
        viewMat = viewMat * camRotMat;

        //=======================================================================
        // Set up transformations and draw the skybox.
        //=======================================================================
        {
            // Actual final camera position in World coordinates.
            glm::vec3 camWorldPos = glm::transpose(glm::mat3(camRotMat)) * glm::vec3(cam_eye[0], cam_eye[1], cam_eye[2]);


            /////////////////////////////////////////////////////////////////////////////
            // Set up the Modelling Matrix modelMat0 for scaling the skybox cube to the
            // size skyboxSize x skyboxSize x skyboxSize, and for positioning it at the
            // correct position. Note that the skybox cube is initially a 1x1x1 cube
            // centered at the origin.
            /////////////////////////////////////////////////////////////////////////////

            glm::mat4 modelMat0 = glm::mat4(1.0f);

            // Remove translation to make the distance infinite.
            modelMat0 = glm::translate(modelMat0, glm::transpose(glm::mat3(camRotMat)) * glm::vec3(cam_eye[0], cam_eye[1], cam_eye[2]));
            // Resize the skybox.
            modelMat0 = glm::scale(modelMat0, glm::vec3(skyboxSize, skyboxSize, skyboxSize));

            glm::mat4 modelViewMat0 = viewMat * modelMat0;
            glm::mat4 modelViewProjMat0 = projMat * modelViewMat0;
            glm::mat3 normalMat0 = glm::transpose(glm::inverse(glm::mat3(modelViewMat0)));

            shaderProg.setUniform("ViewMatrix", viewMat);
            shaderProg.setUniform("ModelViewMatrix", modelViewMat0);
            shaderProg.setUniform("ModelViewProjMatrix", modelViewProjMat0);
            shaderProg.setUniform("NormalMatrix", normalMat0);
            shaderProg.setUniform("WhichObj", 0);  // 0 -- draw skybox, 1 -- draw brick cube, 2 -- draw wooden cube.

            // Draw skybox.
            skyboxModel->render();
        }

        //=======================================================================
        // Set up transformations and other uniform variables and draw the brick cube.
        //=======================================================================
        {
            // Translate it to the left side.
            glm::mat4 modelMat1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

            // Fit to -1, 1
            modelMat1 = glm::scale(modelMat1, glm::vec3(1.5f, 1.5f, 1.5f));

            // Rotate 0 degrees about x-axis.
            modelMat1 = glm::rotate(modelMat1, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 modelViewMat1 = viewMat * modelMat1;
            glm::mat4 modelViewProjMat1 = projMat * modelViewMat1;
            glm::mat3 normalMat1 = glm::transpose(glm::inverse(glm::mat3(modelViewMat1)));

            shaderProg.setUniform("ViewMatrix", viewMat);
            shaderProg.setUniform("ModelViewMatrix", modelViewMat1);
            shaderProg.setUniform("ModelViewProjMatrix", modelViewProjMat1);
            shaderProg.setUniform("NormalMatrix", normalMat1);

            shaderProg.setUniform("LightPosition",
                glm::vec4(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]));
            shaderProg.setUniform("LightAmbient",
                glm::vec4(lightAmbient[0], lightAmbient[1], lightAmbient[2], lightAmbient[3]));
            shaderProg.setUniform("LightDiffuse",
                glm::vec4(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]));
            shaderProg.setUniform("LightSpecular",
                glm::vec4(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]));

            shaderProg.setUniform("WhichObj", 1);  // 0 -- draw skybox, 1 -- draw brick cube, 2 -- draw wooden cube.

            // Draw brick cube.
            obj3DModel1->render();
        }

        //=======================================================================
        // Set up transformations and other uniform variables and draw the wooden cube.
        //=======================================================================
        {
            // Translate it to the right side.
            glm::mat4 modelMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            // Make it slightly larger.
            modelMat2 = glm::scale(modelMat2, glm::vec3(1.5f, 1.5f, 1.5f));

            // Rotate 0 degrees about x-axis.
            modelMat2 = glm::rotate(modelMat2, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 modelViewMat2 = viewMat * modelMat2;
            glm::mat4 modelViewProjMat2 = projMat * modelViewMat2;
            glm::mat3 normalMat2 = glm::transpose(glm::inverse(glm::mat3(modelViewMat2)));

            shaderProg.setUniform("ViewMatrix", viewMat);
            shaderProg.setUniform("ModelViewMatrix", modelViewMat2);
            shaderProg.setUniform("ModelViewProjMatrix", modelViewProjMat2);
            shaderProg.setUniform("NormalMatrix", normalMat2);

            shaderProg.setUniform("LightPosition",
                glm::vec4(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]));
            shaderProg.setUniform("LightAmbient",
                glm::vec4(lightAmbient[0], lightAmbient[1], lightAmbient[2], lightAmbient[3]));
            shaderProg.setUniform("LightDiffuse",
                glm::vec4(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]));
            shaderProg.setUniform("LightSpecular",
                glm::vec4(lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]));

            shaderProg.setUniform("WhichObj", 2);  // 0 -- draw skybox, 1 -- draw brick cube, 2 -- draw wooden cube.

            // Draw wooden cube.
            obj3DModel2->render();
        }
	}

	void TestCube::OnImGuiRender()
	{
		ImGui::Text("Cube");
	}

    void TestCube::SetupEnvMap(void)
    {
        const int numImages = 6;
        const GLenum texUnit = GL_TEXTURE0;

        // Cubemap images' filenames.
        const char* cubemapFile[numImages] = {
            "res/images/cm2_right.png", "res/images/cm2_left.png",
            "res/images/cm2_top.png", "res/images/cm2_bottom.png",
            "res/images/cm2_back.png", "res/images/cm2_front.png"
        };


        GLuint target[numImages] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };


        glActiveTexture(texUnit);
        GLuint tid;
        glGenTextures(1, &tid);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Enable fliping of images vertically when read in.
        // This is to follow OpenGL's image coordinate system, i.e. bottom-leftmost is (0, 0).
        stbi_set_flip_vertically_on_load(true);

        // Read texture images from files.
        for (int t = 0; t < numImages; t++) {

            int imgWidth, imgHeight, numComponents;

            GLubyte* imgData = stbi_load(cubemapFile[t], &imgWidth, &imgHeight, &numComponents, 0);

            if (imgData == NULL) {
                fprintf(stderr, "Error: Fail to read image file %s.\n", cubemapFile[t]);
                exit(EXIT_FAILURE);
            }
            printf("%s (%d x %d, %d components)\n", cubemapFile[t], imgWidth, imgHeight, numComponents);

            if (numComponents == 1) {
                glTexImage2D(target[t], 0, GL_R8, imgWidth, imgHeight, 0,
                    GL_RED, GL_UNSIGNED_BYTE, imgData);
            }
            else if (numComponents == 3) {
                glTexImage2D(target[t], 0, GL_RGB8, imgWidth, imgHeight, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, imgData);
            }
            else {
                fprintf(stderr, "Error: Unexpected image format.\n");
                exit(EXIT_FAILURE);
            }

            stbi_image_free(imgData);
        }

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void TestCube::SetupTextureMaps(void)
    {
        const int numTexMaps = 3;

        const char* texMapFile[numTexMaps] = {
            "res/images/brick_diffuse_map.png",
            "res/images/brick_normal_map.png",
            "res/images/wood_diffuse_map.png"
        };

        const GLenum texUnit[numTexMaps] = { GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };

        // Enable fliping of images vertically when read in.
        // This is to follow OpenGL's image coordinate system, i.e. bottom-leftmost is (0, 0).
        stbi_set_flip_vertically_on_load(true);

        // Read texture images from files.
        for (int t = 0; t < numTexMaps; t++) {

            int imgWidth, imgHeight, numComponents;

            GLubyte* imgData = stbi_load(texMapFile[t], &imgWidth, &imgHeight, &numComponents, 0);

            if (imgData == NULL) {
                fprintf(stderr, "Error: Fail to read image file %s.\n", texMapFile[t]);
                exit(EXIT_FAILURE);
            }
            printf("%s (%d x %d, %d components)\n", texMapFile[t], imgWidth, imgHeight, numComponents);

            glActiveTexture(texUnit[t]);
            GLuint tid;
            glGenTextures(1, &tid);
            glBindTexture(GL_TEXTURE_2D, tid);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            if (numComponents == 1) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, imgWidth, imgHeight, 0,
                    GL_RED, GL_UNSIGNED_BYTE, imgData);
            }
            else if (numComponents == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imgWidth, imgHeight, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, imgData);
            }
            else {
                fprintf(stderr, "Error: Unexpected image format.\n");
                exit(EXIT_FAILURE);
            }

            stbi_image_free(imgData);

            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

	
} // namespace test