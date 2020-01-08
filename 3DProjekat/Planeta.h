#define PLANETA_H
#include <GLFW/glfw3.h>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include </Brze Stvari/Grafika/3DProjekat/3DProjekat/Shader.h>
#include </Brze Stvari/Grafika/3DProjekat/3DProjekat/Mesh.h>
#include </Brze Stvari/Grafika/3DProjekat/3DProjekat/Model.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Planeta {
public:

	Model modelPlanete;
	glm::mat4 model;
	string imePlanete;
	Shader planetaShader;
	
	Planeta(Shader planetaShader, string imePlanete, Model modelPlanete, float zaTranslirati, float zaSkalirati) : modelPlanete("assets/objekti/planet/merkur/planet.obj"), planetaShader("shaders/planeta.vs", "shaders/planeta.fs"){

		this->imePlanete = imePlanete;
		this->modelPlanete = modelPlanete;
		this->zaTranslirati = zaTranslirati;
		this->zaSkalirati = zaSkalirati;
		this->planetaShader = planetaShader;
		


	}

	void Update() {
		float currentFrame = glfwGetTime();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(currentFrame), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::translate(model, glm::vec3(zaTranslirati, 0, 0));                                     //Pomeranje planete u odnosu na sunce

		model = glm::scale(model, glm::vec3(zaSkalirati, zaSkalirati, zaSkalirati));                                   //Skaliranje u odnosu  na ostale planete
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));                  //Rotiranje za 90 stepeni kako bi polovi bili dobro orijentisani
		if (imePlanete.compare("mesec") == 0) {
			model = glm::rotate(model, glm::radians(currentFrame), glm::vec3(0.0f, 0.0f, 1.0f));            //Rotiranje oko zemlje
			model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
		}
		model = glm::rotate(model, glm::radians(currentFrame / 2), glm::vec3(0.0f, 0.0f, 1.0f));        //Rotiranje oko sopstvene ose
		//std::cout << glm::radians(currentFrame / 2) << std::endl;
		planetaShader.setMat4("model", model);
		modelPlanete.Draw(planetaShader);
	}


private:
	float zaTranslirati;
	float zaSkalirati;



};

