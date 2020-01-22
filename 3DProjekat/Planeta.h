#define PLANETA_H
#include <GLFW/glfw3.h>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include </Users/Filip/Desktop/Projekti iz grafike/Drugi projekat/3DProjekat/3DProjekat/Shader.h>
#include </Users/Filip/Desktop/Projekti iz grafike/Drugi projekat/3DProjekat/3DProjekat/Mesh.h>
#include </Users/Filip/Desktop/Projekti iz grafike/Drugi projekat/3DProjekat/3DProjekat/Model.h>

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
	
	Planeta(Shader planetaShader, string imePlanete, Model modelPlanete, float zaTranslirati, float zaSkalirati, float brzinaRotiranjaOkoSunca = 1.0f, float nagnuce = 0.0f) : modelPlanete("assets/objekti/planet/merkur/planet.obj"), planetaShader("shaders/planeta.vs", "shaders/planeta.fs"){

		this->imePlanete = imePlanete;
		this->modelPlanete = modelPlanete;
		this->zaTranslirati = zaTranslirati;
		this->zaSkalirati = zaSkalirati;
		this->planetaShader = planetaShader;
		this->brzinaRotiranjaOkoSunca = brzinaRotiranjaOkoSunca;
		this->nagnuce = nagnuce;

	}

	void Update(Shader shader, bool ravnazemlja, bool rotacija) {
		if (ravnazemlja) {
			

				float currentFrame = glfwGetTime();
				model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(currentFrame) * brzinaRotiranjaOkoSunca, glm::vec3(0.0f, 1.0f, 0.0f));			//Rotiranje oko sunca

				model = glm::translate(model, glm::vec3(zaTranslirati, 0, 0));                                     //Pomeranje planete u odnosu na sunce

				if(imePlanete.compare("sunce") != 0)
					model = glm::scale(model, glm::vec3(zaSkalirati, zaSkalirati, 0.0f));                                   //Skaliranje u odnosu  na ostale planete
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));                  //Rotiranje za 90 stepeni kako bi polovi bili dobro orijentisani
				if (imePlanete.compare("mesec") == 0) {
					model = glm::rotate(model, glm::radians(currentFrame), glm::vec3(0.0f, 0.0f, 1.0f));            //Rotiranje oko zemlje
					model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));									//Transliranje od zemlje
				}
				if (imePlanete.compare("sunce") != 0)
					model = glm::rotate(model, glm::radians(currentFrame / 2), glm::vec3(0.0f, 0.0f, 0.1f));        //Rotiranje oko sopstvene ose
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));                  //Rotiranje za 90 stepeni kako bi polovi bili dobro orijentisani

				//std::cout << glm::radians(currentFrame / 2) << std::endl;
				planetaShader.setMat4("model", model);
				modelPlanete.Draw(planetaShader);
			
		} else {
			
			

				float currentFrame = glfwGetTime();
				model = glm::mat4(1.0f);
				if(rotacija == true)
					model = glm::rotate(model, glm::radians(currentFrame) * brzinaRotiranjaOkoSunca, glm::vec3(0.0f, 1.0f, 0.0f));			//Rotiranje oko sunca

				model = glm::translate(model, glm::vec3(zaTranslirati, 0, 0));                                     //Pomeranje planete u odnosu na sunce

				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));                  //Rotiranje za 90 stepeni kako bi polovi bili dobro orijentisani
				if (imePlanete.compare("mesec") == 0) {
					model = glm::rotate(model, glm::radians(currentFrame), glm::vec3(0.0f, 0.0f, 1.0f));            //Rotiranje oko zemlje
					model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));									//Transliranje od zemlje
				}
				if (imePlanete.compare("sunce") != 0)
					if (imePlanete.compare("jupiter") == 0) {
						model = glm::rotate(model, glm::radians(currentFrame*10), glm::vec3(0.0f, 0.0f, 1.0f));        //Rotiranje oko sopstvene ose
						

					}
					else
						model = glm::rotate(model, glm::radians(currentFrame / 2), glm::vec3(0.0f, 0.0f, 0.1f));        //Rotiranje oko sopstvene ose
			
				model = glm::rotate(model, glm::radians(nagnuce), glm::vec3(1.0f, 0.0f, 0));        //tiltovanje
				model = glm::scale(model, glm::vec3(zaSkalirati, zaSkalirati, zaSkalirati));                                   //Skaliranje u odnosu  na ostale planete

				shader.setMat4("model", model);

			

				modelPlanete.Draw(shader);
			
		}


	}

	

private:
	float zaTranslirati;
	float zaSkalirati;
	float brzinaRotiranjaOkoSunca;
	float nagnuce;

};

