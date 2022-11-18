#include "Testgridmap.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include "stb_image/stb_image.h"
#include "json_parser/JsonParser.h"

#include "scene/Gridmap.h"
#include "scene/Car.h"

#include "vector"
#include "chrono"
#include "thread"
#include "string"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace jsonxx;
using namespace std;

namespace test {
    // static variables initialization
    // Light info.
    GLfloat TestGridmap::initial_lightPosition[] = { 5.0f, 3.0f, 10.0f, 0.0f };  // Directional light. Assumed in eye space.
    GLfloat TestGridmap::lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat TestGridmap::lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat TestGridmap::lightSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat TestGridmap::lightPosition[4];  // Directional light. Assumed in eye space.

    float TestGridmap::lightPosIncr = 0.5f; // Step size to move light position in x & y directions.

    float TestGridmap::skyboxSize = 50.0f;  // Desired width of the skybox.

    // For window and viewport size.
    int TestGridmap::winWidth = 800;     // Window width in pixels.
    int TestGridmap::winHeight = 600;    // Window height in pixels.

    bool TestGridmap::wireframeMode;

    // For trackball.
    double TestGridmap::prevMouseX, TestGridmap::prevMouseY;
    bool TestGridmap::mouseLeftPressed;
    bool TestGridmap::mouseMiddlePressed;
    bool TestGridmap::mouseRightPressed;
    float TestGridmap::cam_curr_quat[4];
    float TestGridmap::cam_prev_quat[4];
    float TestGridmap::cam_eye[3], TestGridmap::cam_lookat[3], TestGridmap::cam_up[3];
    float TestGridmap::initial_cam_pos[3] = { 0.0, 7.0, 8.0f };  // World coordinates.

    // Grid map
    Gridmap* wholeGridmap;

    TestGridmap::TestGridmap()
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
        skyboxModel = make_shared<VBOCube>();
        obj3DModel = shared_ptr<VBOCube[]>(new VBOCube[300]);

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

        // read map data
        string mapData;
        ifstream file;
        string mapDataPath = R"(src\data\mapData.txt)";
        file.open(mapDataPath, ios_base::in);
        if (!file.is_open())
        {
            cout << "[LOGINFO]file does'nt exist";
        }
        getline(file, mapData);
        file.close();

        //=======================================================================
        // Parse gridmap params.
        //=======================================================================
        {
            Object gridmapObj;
            gridmapObj.parse(mapData);
            int width = gridmapObj.get<Number>("mapCols");
            int height = gridmapObj.get<Number>("mapCols");
            string heightMap = gridmapObj.get<String>("mapMat");
            wholeGridmap = new Gridmap(width, height);
            for (int i = 0; i < wholeGridmap->GetHeight(); i++)
            {
                for (int j = 0; j < wholeGridmap->GetWidth(); j++)
                {
                    int height = heightMap[i * wholeGridmap->GetWidth() + j] - '0' + 1;
                    wholeGridmap->SetHeight(i, j, height);
                }
            }
        }

        //=======================================================================
        // Parse car params.
        //=======================================================================
        vector<string> carArr;
        string carData;
        string carDataPath = R"(src\data\carData.txt)";
        file.open(carDataPath, ios_base::in);
        if (!file.is_open())
        {
            cout << "[LOGINFO]file does'nt exist";
        }
        while (getline(file, carData)) {
            carArr.push_back(carData);
        };
        int maxLength = 0;
        file.close();
        {
            for (string& carData : carArr)
            {
                Object carObj;
                carObj.parse(carData);
                // get type
                int type = carObj.get<Number>("type");

                int sColor = rand() % 3;
                Car::CarColor color;
                if (sColor == 0)color = Car::CarColor::RED;
                if (sColor == 1)color = Car::CarColor::GREEN;
                if (sColor == 2)color = Car::CarColor::BLUE;

                int pathLength = carObj.get<Number>("pathLength");
                maxLength = max(maxLength, pathLength);

                vector<Car::CarPosition> positions;
                for (int i = 0; i < pathLength; i++)
                {
                    float x = carObj.get<Array>("rows").get<Number>(i);
                    float y = carObj.get<Array>("cols").get<Number>(i);
                    Car::CarPosition pos;
                    if (type == 2)pos = { x,y,0 };
                    else
                    {
                        pos = { x / 1000, y / 1000 ,0 };
                    }
                    positions.push_back(pos);
                }
                // fixed size
                Car::CarSize size = { 1,1,1 };
                Car* car = new Car(Car::CarType::BUS, color, positions, size);
                wholeGridmap->AddCar(*car);
            }
        }


        // interpolation
        for (auto& car : wholeGridmap->m_Cars)
        {
            if (car.m_Positions.size() < maxLength)
            {
                int ct = maxLength - car.m_Positions.size();
                std::vector<Car::CarPosition> tmpPositions;
                while (tmpPositions.size() < maxLength) 
                {
                    for (int i = 0; i < car.m_Positions.size()-1;i++)
                    {
                        Car::CarPosition pos = { (car.m_Positions[i].x + car.m_Positions[i + 1].x) / 2.0, (car.m_Positions[i].y + car.m_Positions[i + 1].y) / 2 ,0 };
                        tmpPositions.push_back(car.m_Positions[i]);
                        if (tmpPositions.size() == maxLength)break;
                        tmpPositions.push_back(pos);
                        if (tmpPositions.size() == maxLength)break;
                    }
                    car.m_Positions = tmpPositions;
                }
            }
            cout << car.m_Positions.size() << endl;
        }
        auto xx= wholeGridmap->m_Cars;

    }



    TestGridmap::~TestGridmap()
    {
        //delete skyboxModel;
		//delete  obj3DModel;
    }

    void TestGridmap::OnUpdate(float deltaTime)
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

            if (TestGridmap::mouseLeftPressed) {
                trackball(TestGridmap::cam_prev_quat, rotScale * (2.0f * TestGridmap::prevMouseX - TestGridmap::winWidth) / (float)TestGridmap::winWidth,
                    rotScale * (TestGridmap::winHeight - 2.0f * TestGridmap::prevMouseY) / (float)TestGridmap::winHeight,
                    rotScale * (2.0f * mouse_x - TestGridmap::winWidth) / (float)TestGridmap::winWidth,
                    rotScale * (TestGridmap::winHeight - 2.0f * mouse_y) / (float)TestGridmap::winHeight);

                add_quats(TestGridmap::cam_prev_quat, TestGridmap::cam_curr_quat, TestGridmap::cam_curr_quat);
            }
            else if (TestGridmap::mouseMiddlePressed) {
                TestGridmap::cam_eye[0] -= transScale * (mouse_x - TestGridmap::prevMouseX) / (float)TestGridmap::winWidth;
                TestGridmap::cam_lookat[0] -= transScale * (mouse_x - TestGridmap::prevMouseX) / (float)TestGridmap::winWidth;
                TestGridmap::cam_eye[1] += transScale * (mouse_y - TestGridmap::prevMouseY) / (float)TestGridmap::winHeight;
                TestGridmap::cam_lookat[1] += transScale * (mouse_y - TestGridmap::prevMouseY) / (float)TestGridmap::winHeight;
            }
            else if (TestGridmap::mouseRightPressed) {
                TestGridmap::cam_eye[2] += transScale * (mouse_y - TestGridmap::prevMouseY) / (float)TestGridmap::winHeight;
                TestGridmap::cam_lookat[2] += transScale * (mouse_y - TestGridmap::prevMouseY) / (float)TestGridmap::winHeight;
            }

            // Update mouse point
            TestGridmap::prevMouseX = mouse_x;
            TestGridmap::prevMouseY = mouse_y;
        }

        // Mouse Click
        {
            int action;
            if (action = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT))
            {
                TestGridmap::mouseLeftPressed = true;
                trackball(TestGridmap::cam_prev_quat, 0.0, 0.0, 0.0, 0.0);
            }
            else if (action = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT))
            {
                TestGridmap::mouseRightPressed = true;
            }
            else if (action = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE))
            {
                TestGridmap::mouseMiddlePressed = true;
            }
            else
            {
                TestGridmap::mouseLeftPressed = false;
                TestGridmap::mouseMiddlePressed = false;
                TestGridmap::mouseRightPressed = false;
            }
        }
        // Update cars' positions
        wholeGridmap->UpdateCarsPosition();
        cout << "[LOGINFO]Car position update" << endl;

        // Sleep
        this_thread::sleep_for(chrono::milliseconds((int)deltaTime)); 
    }

    void TestGridmap::OnRender()
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
        // Draw the grid maps.
        //=======================================================================
        for (int i = 0; i < wholeGridmap->GetHeight(); i++)
        {
            for (int j = 0; j < wholeGridmap->GetHeight(); j++)
            {
                // Scale size
                float scaleSize = 0.2;

                // Translation
                glm::mat4 modelMat1 = glm::translate(glm::mat4(1.0f), glm::vec3(scaleSize * i, scaleSize * wholeGridmap->GetHeight(i, j) / 2.0, scaleSize * j));
                
                modelMat1 = glm::scale(scaleSize*modelMat1, glm::vec3(scaleSize * 1.0f, scaleSize * wholeGridmap->GetHeight(i, j), scaleSize * 1.0f));

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


                // Draw the grid map
                if (wholeGridmap->GetHeight(i,j)==1)
                {
                    shaderProg.setUniform("WhichObj", 3);  
                }
                else
                {
                    shaderProg.setUniform("WhichObj", 4);  
                }
                // Draw
                (obj3DModel.get() + i)->render();

            }
        }

        //=======================================================================
        // Draw the cars.
        //=======================================================================
        for(int i=0;i< wholeGridmap->GetCars().size();i++)
        {
            auto car = wholeGridmap->GetCars()[i];
            // Scale size
            float scaleSize = 0.2;

            // Translate it to the left side.
            glm::mat4 modelMat1 = glm::translate(glm::mat4(1.0f), glm::vec3(scaleSize * car.m_Positions[car.m_Iter].x, scaleSize * 3.0 / 2.0, scaleSize * car.m_Positions[car.m_Iter].y));

            // Output the position
            cout<< "[LOGINFO]Car" << i << "'s position:" << car.m_Positions[car.m_Iter].x << " " << car.m_Positions[car.m_Iter].y << endl;

            //scaleSize = 2;
            modelMat1 = glm::scale(scaleSize * modelMat1, glm::vec3(scaleSize * car.m_Size.scale_x * 1.0f, scaleSize * car.m_Size.scale_z / 2.0, scaleSize * car.m_Size.scale_y * 1.0f));

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

            switch (car.m_Color)
            {
            case Car::CarColor::RED:
                shaderProg.setUniform("WhichObj", 5);
                break;
            case Car::CarColor::GREEN:
                shaderProg.setUniform("WhichObj", 6);
                break;
            default:
                shaderProg.setUniform("WhichObj", 7);
                break;
            }
            // Draw
            (obj3DModel.get())->render();
        }
    }

    void TestGridmap::OnImGuiRender()
    {
        ImGui::Text("System is running...");
        auto cars = wholeGridmap->GetCars();
        for (int i = 0; i < cars.size(); i++)
        {
            Car car = cars[i];
            string carId = "Car Id:"+to_string(i);
            ImGui::Text(carId.data());
            // print car color
            string carColor;
            switch (car.m_Color)
            {
            case Car::CarColor::RED:
                carColor = "  Car Color:Red";
                break;
            case Car::CarColor::GREEN:
                carColor = "  Car Color:Green";
                break;
            default:
                carColor = "  Car Color:Blue";
                break;
            }
            ImGui::Text(carColor.data());
            // print car type
            string carType;
            switch (car.m_Type)
            {
            case Car::CarType::BUS:
                carType = "  Car Type:Bus";
                break;
            case Car::CarType::CAR:
                carType = "  Car Type:Car";
                break;
            default:
                carType = "  Car Type:Truck";
                break;
            }
            ImGui::Text(carType.data());
        }
    }

    void TestGridmap::SetupEnvMap(void)
    {
        const int numImages = 6;
        const GLenum texUnit = GL_TEXTURE0;

        // Cubemap images' filenames.
        const char* cubemapFile[numImages] = {
            "res/images/dot_right.jpg", "res/images/dot_left.jpg",
            "res/images/dot_top.jpg", "res/images/dot_bottom.jpg",
            "res/images/dot_back.jpg", "res/images/dot_front.jpg"
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

    void TestGridmap::SetupTextureMaps(void)
    {
        const int numTexMaps = 3;

        const char* texMapFile[numTexMaps] = {
            "res/images/grass.png",
            "res/images/grass.png",
            "res/images/brick_diffuse_map.png"
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