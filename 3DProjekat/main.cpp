#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include </Brze Stvari/Grafika/3DProjekat/3DProjekat/Shader.h>
#include </Brze Stvari/Grafika/3DProjekat/3DProjekat/model.h>
#include </Brze Stvari/Grafika/3DProjekat/3DProjekat/Planeta.h>

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource;
const char* fragmentShaderSource;

glm::vec3 cameraPos = glm::vec3(0.0f, 250.0f, -3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = 0.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

struct Kocka {
    glm::vec3 pos, speed;
    float size, angle, weight;
    float life;
};


const int MaxBrojKocki = 10;
Kocka KockeContainer[MaxBrojKocki];
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

    std::vector<Planeta> planete;
    planete.reserve(11);

    Shader asteroidShader("shaders/asteroid.vs", "shaders/asteroid.fs");
    Shader planetaShader("shaders/planeta.vs", "shaders/planeta.fs");
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
	
    //Shader ourShader("shaders/svetlo_shader.vs", "shaders/svetlo_shader.fs");
    Model rockModel("assets/objekti/rock/rock.obj");

    Model sunceModel("assets/objekti/planet/sunce/planet.obj");
    Planeta sunce(planetaShader, "sunce", sunceModel, 0.0f, 100.0f);
    planete.push_back(sunce);
    printf("Ucitano sunce\n");
    Model merkurModel("assets/objekti/planet/merkur/planet.obj");
    Planeta merkur(planetaShader, "merkur", merkurModel, 300.0f, 0.40f);
    planete.push_back(merkur);
    printf("Ucitan merkur\n");
    Model veneraModel("assets/objekti/planet/venera/planet.obj");
    Planeta venera(planetaShader, "venera", veneraModel, 400.0f, 0.90f);
    planete.push_back(venera);
    printf("Ucitana venera\n");
    Model zemljaModel("assets/objekti/planet/zemlja/planet.obj");
    Planeta zemlja(planetaShader, "zemlja", zemljaModel, 500.0f, 1.0f);
    planete.push_back(zemlja);
    printf("Ucitana zemlja\n");
    Model moonModel("assets/objekti/planet/moon/planet.obj");
    Planeta moon(planetaShader, "mesec", moonModel, 500.0f, 0.30f);
    planete.push_back(moon);
    printf("Ucitan mesec\n");
    Model marsModel("assets/objekti/planet/mars/planet.obj");
    Planeta mars(planetaShader, "mars", marsModel, 560.0f, 0.60f);
    planete.push_back(mars);
    printf("Ucitan mars\n");
    Model jupiterModel("assets/objekti/planet/jupiter/planet.obj");
    Planeta jupiter(planetaShader, "jupiter", jupiterModel, 620.0f, 10.30f);
    planete.push_back(jupiter);
    printf("Ucitan jupiter\n");
    Model saturnModel("assets/objekti/planet/saturn/planet.obj");
    Planeta saturn(planetaShader, "saturn", saturnModel, 700.0f, 8.30f);
    planete.push_back(saturn);
    printf("Ucitan saturn\n");
    Model uranModel("assets/objekti/planet/uran/planet.obj");
    Planeta uran(planetaShader, "uran", uranModel, 750.0f, 3.30f);
    planete.push_back(uran);
    printf("Ucitan uran\n");
    Model neptuneModel("assets/objekti/planet/neptune/planet.obj");
    Planeta neptune(planetaShader, "neptune", neptuneModel, 800.0f, 3.30f);
    planete.push_back(neptune);
    printf("Ucitan neptun\n");
    Model planetModel("assets/objekti/planet/planeta/planet.obj");
    Planeta planet(planetaShader, "planet", planetModel, 900.0f, 4.30f);
    planete.push_back(planet);
    printf("Ucitana random planeta \n");


    unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    

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
            "assets/objekti/ama/right.png",
            "assets/objekti/ama/left.png",
            "assets/objekti/ama/up.png",
            "assets/objekti/ama/down.png",
            "assets/objekti/ama/front.png",
            "assets/objekti/ama/back.png"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);


    

    //glfwSwapInterval(1);
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        
        planetaShader.use();
        planetaShader.setMat4("projection", projection);
        planetaShader.setMat4("view", view);


        //PLANETE
        for (unsigned int i = 0; i < 11; i++) {
            planete[i].Update();
            
        }


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



        ////ASTEROIDI OKO RANDOM PLANETE
        //for (unsigned int i = 0; i < amount; i++) {

        //    glm::mat4 model = glm::mat4(1.0f);

        //    model = glm::rotate(model, glm::radians(currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));
        //    model = glm::translate(model, glm::vec3(900.0f, 0.0f, 0.0f));
        //    model = glm::rotate(model, glm::radians((float)randomBrojevi[i]), glm::vec3(0.0f, 1.0f, 0.0f));
        //    model = glm::translate(model, glm::vec3(50.0f + (randomBrojevi[i] % 20), 0.0f, 0.0f));
        //    model = glm::scale(model, glm::vec3(randomBrojevi[i] % 50 /50.0f));
        //
        //    modelMatrices[i] = model;

        //}
        //unsigned int buffer;
        //glGenBuffers(1, &buffer);
        //glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        //asteroidShader.use();
        //asteroidShader.setMat4("projection", projection);
        //asteroidShader.setMat4("view", view);
        //asteroidShader.setInt("texture_diffuse1", 0);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, rockModel.textures_loaded[0].id);

        //for (unsigned int i = 0; i < rockModel.meshes.size(); i++) {
        //    unsigned int VAO = rockModel.meshes[i].VAO;
        //    glBindVertexArray(VAO);
        //    // set attribute pointers for matrix (4 times vec4)
        //    glEnableVertexAttribArray(3);
        //    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        //    glEnableVertexAttribArray(4);
        //    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        //    glEnableVertexAttribArray(5);
        //    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        //    glEnableVertexAttribArray(6);
        //    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        //    glVertexAttribDivisor(3, 1);
        //    glVertexAttribDivisor(4, 1);
        //    glVertexAttribDivisor(5, 1);
        //    glVertexAttribDivisor(6, 1);

        //    glBindVertexArray(0);
        //    
        //    
        //    glBindVertexArray(rockModel.meshes[i].VAO);
        //    glDrawElementsInstanced(GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        //    glBindVertexArray(0);
        //}








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



void processInput(GLFWwindow* window) {
    float cameraSpeed = 0.1f;



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
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
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
