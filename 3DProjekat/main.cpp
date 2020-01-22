#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include </Users/Filip/Desktop/Projekti iz grafike/Drugi projekat/3DProjekat/3DProjekat/Shader.h>
#include </Users/Filip/Desktop/Projekti iz grafike/Drugi projekat/3DProjekat/3DProjekat/model.h>
#include </Users/Filip/Desktop/Projekti iz grafike/Drugi projekat/3DProjekat/3DProjekat/Planeta.h>

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);
void renderScene(const Shader& shader);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const char* vertexShaderSource;
const char* fragmentShaderSource;

glm::vec3 cameraPos = glm::vec3(0.0f, 250.0f, -3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = 0.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 70.0f;

bool ravnazemlja = false;
bool rotacija = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::vector<Planeta> planete;

bool shadows = false;


unsigned int randomBrojevi[1000] = { 0 };
int main() {

    for (int i = 0; i < 1000; i++) {
        randomBrojevi[i] = (int)rand();
       // std::cout << randomBrojevi[i] << " \n" << std::endl;
    }
    
    
    

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Univerzum", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    planete.reserve(11);

    Shader asteroidShader("shaders/asteroid.vs", "shaders/asteroid.fs");


    Shader depthShader("shaders/shadowDepth.vs", "shaders/shadowDepth.fs", "shaders/shadowDepth.gs");
    printf("Ucitani depth shader\n");

    Shader planetaShader("shaders/planetaShadow.vs", "shaders/planetaShadow.fs");
    printf("Ucitani planeta shader\n");
    
    Shader sunceShader("shaders/planeta.vs", "shaders/planeta.fs");
    printf("Ucitan sunce shader\n");

    Shader zemljaShader("shaders/zemljaShader.vs", "shaders/zemljaShader.fs");
    printf("Ucitan zemlja shader\n");

    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    

    printf("Ucitani svi shaderi");

    //Shader ourShader("shaders/svetlo_shader.vs", "shaders/svetlo_shader.fs");
    Model rockModel("assets/objekti/rock/rock.obj");

    Model sunceModel("assets/objekti/planet/sunce/planet.obj");
    Planeta sunce(planetaShader, "sunce", sunceModel, 0.0f, 100.0f, 0.5);
    planete.push_back(sunce);
    printf("Ucitano sunce\n");
    Model zemljaModel("assets/objekti/planet/zemlja/planet.obj");
    Planeta zemlja(planetaShader, "zemlja", zemljaModel, 550.0f, 1.0f, 1.0f,23.0f);
    planete.push_back(zemlja);
    printf("Ucitana zemlja\n");
    Model moonModel("assets/objekti/planet/moon/planet.obj");
    Planeta moon(planetaShader, "mesec", moonModel, 550.0f, 0.30f, 1.0f);
    planete.push_back(moon);
    printf("Ucitan mesec\n");
    Model merkurModel("assets/objekti/planet/merkur/planet.obj");
    Planeta merkur(planetaShader, "merkur", merkurModel, 400.0f, 0.40f, 1.6f);
    planete.push_back(merkur);
    printf("Ucitan merkur\n");
    Model veneraModel("assets/objekti/planet/venera/planet.obj");
    Planeta venera(planetaShader, "venera", veneraModel, 450.0f, 0.90f, 1.17f);
    planete.push_back(venera);
    printf("Ucitana venera\n");
    
    Model marsModel("assets/objekti/planet/mars/planet.obj");
    Planeta mars(planetaShader, "mars", marsModel, 610.0f, 0.60f, 0.8f, 25.0f);
    planete.push_back(mars);
    printf("Ucitan mars\n");
    Model jupiterModel("assets/objekti/planet/jupiter/planet.obj");
    Planeta jupiter(planetaShader, "jupiter", jupiterModel, 670.0f, 10.30f, 0.5f, 3.0f);
    planete.push_back(jupiter);
    printf("Ucitan jupiter\n");
    Model saturnModel("assets/objekti/planet/saturn/planet.obj");
    Planeta saturn(planetaShader, "saturn", saturnModel, 750.0f, 8.30f, 0.33f, 27.0f);
    planete.push_back(saturn);
    printf("Ucitan saturn\n");
    Model uranModel("assets/objekti/planet/uran/planet.obj");
    Planeta uran(planetaShader, "uran", uranModel, 850.0f, 3.30f, 0.23f, 90.0f);
    planete.push_back(uran);
    printf("Ucitan uran\n");
    Model neptuneModel("assets/objekti/planet/neptune/planet.obj");
    Planeta neptune(planetaShader, "neptune", neptuneModel, 900.0f, 3.30f, 0.16f, 30.0f);
    planete.push_back(neptune);
    printf("Ucitan neptun\n");
    
    
    Model planetModel("assets/objekti/planet/planeta/planet.obj");
    Planeta planet(planetaShader, "planet", planetModel, 950.0f, 4.30f, 0.5f);
    planete.push_back(planet);

    printf("Ucitana random planeta \n");


    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };



    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<std::string> faces
    {
            "assets/objekti/milkyway/right.png",
            "assets/objekti/milkyway/left.png",
            "assets/objekti/milkyway/up.png",
            "assets/objekti/milkyway/down.png",
            "assets/objekti/milkyway/front.png",
            "assets/objekti/milkyway/back.png"
    };

    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);



    planetaShader.setInt("diffuseTexture", 0);
    planetaShader.setInt("depthMap", 1);

    const unsigned int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);                                                //Koristimo depthmapu za generisanje senki
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);                                       
    for (unsigned int i = 0; i < 6; ++i)                                            //Na svaku od 6 stranica kocke lepimo texturu
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);           
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);                                                           //Zanima nas samo dubina tako da moramo da kazemo OpenGLu da ne renderuje boje
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);



    unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    glm::mat4* stockModelMatrices;
    float skejlovi[1000] = {};
    modelMatrices = new glm::mat4[amount];
    stockModelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) + displacement;
        model = glm::translate(model, glm::vec3(0, y, 0));

        // 2. scale: Scale between 0.05 and 0.25f
        skejlovi[i] = (rand() % 20) / 100.0f + 0.05;
        //model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        //model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
        stockModelMatrices[i] = model;
    }

    // configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < rockModel.meshes.size(); i++) {
        unsigned int VAO = rockModel.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }






    glfwSwapInterval(1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        // input
          // -----

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //std::cout << "delta time = " << deltaTime << std::endl;

        processInput(window);

        

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
        float near_plane = 10.0f;
        float far_plane = 1100.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect , near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

     


        // Renderuje se Depth mapa 

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        depthShader.use();
        for (unsigned int i = 0; i < 6; i++) {
            depthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        }


        depthShader.setFloat("far_plane", far_plane);
        depthShader.setVec3("lightPos", lightPos);
        renderScene(depthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // Normalno se renderuje scena

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        planetaShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



        planetaShader.setMat4("projection", projection);
        planetaShader.setMat4("view", view);

        planetaShader.setVec3("lightPos", lightPos);
        planetaShader.setVec3("viewPos", cameraPos);
        planetaShader.setInt("shadows", shadows); 
        planetaShader.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        
        ////PLANETE
        for (unsigned int i = 3; i < 11; i++) {
            planete[i].Update(planetaShader, ravnazemlja, rotacija);

        }


        zemljaShader.use();
        zemljaShader.setMat4("projection", projection);
        zemljaShader.setMat4("view", view);
        planete[1].Update(zemljaShader, ravnazemlja, rotacija);

        planete[2].Update(zemljaShader, ravnazemlja, rotacija);

        sunceShader.use();
        sunceShader.setMat4("projection", projection);
        sunceShader.setMat4("view", view);
        planete[0].Update(sunceShader, ravnazemlja, rotacija);



        glm::mat4 model = glm::mat4(1.0f);

        for (int i = 0; i < amount; i++) {

            float displacement = randomBrojevi[i] % (int)offset;
            model = glm::rotate(stockModelMatrices[i], glm::radians(currentFrame) /2, glm::vec3(0.0f, 1.0f, 0.0f));

            model = glm::translate(model, glm::vec3(950.0f, 0, 0));

            model = glm::rotate(model, glm::radians(-currentFrame)-randomBrojevi[i], glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(displacement+50, 0, displacement+50));
            model = glm::rotate(model, glm::radians(currentFrame) + displacement+500, glm::vec3(1.0f, 1.0f, 0.0f));

            modelMatrices[i] = glm::scale(model, glm::vec3(skejlovi[i]));

        }

        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);


        for (unsigned int i = 0; i < rockModel.meshes.size(); i++) {
            unsigned int VAO = rockModel.meshes[i].VAO;
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }


        asteroidShader.use();
        asteroidShader.setInt("texture_diffuse1", 0);
        asteroidShader.setMat4("projection", projection);
        asteroidShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rockModel.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        for (unsigned int i = 0; i < rockModel.meshes.size(); i++) {
            glBindVertexArray(rockModel.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }

        

       //sunce.Update();


        // SKYBOX
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp))); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default



      



       








		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);


	glfwTerminate();
	return 0;
}



void renderScene(const Shader& shader) {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    
    
  /*  shader.setMat4("projection", projection);
    shader.setMat4("view", view);*/


    //PLANETE
    for (unsigned int i = 10; i > 3; i--) {
        
        planete[i].Update(shader, ravnazemlja, rotacija);

       
    }



}



void processInput(GLFWwindow* window) {
    float cameraSpeed = 0.5f;



	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront * 10.0f;
        //std::cout << "Pritisnut shift i jos neko dugme" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront * 10.0f;
        //std::cout << "Pritisnut shift i jos neko dugme" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
   
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        ravnazemlja = false;
       
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        ravnazemlja = true;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (rotacija == true) {
            rotacija = false;
        }
        else rotacija = true;
    }


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {



    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}


unsigned int loadCubemap(vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    printf("faces.size %d \n", faces.size());
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
            );
            printf("Ucitana %d-ta slika po redu \n", i);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    std::cout << textureID << std::endl;
    return textureID;
}
