/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include "Scene.hpp"
#include <iostream>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

using namespace std;
namespace E2_3DA
{
	//El constructor de escena requiere de un Skybox y un postprocesado unico
	Scene::Scene(int width, int height) : 
		skybox     ("../../assets/textures/purplenebula-"          ), 
		postprocess("../../assets/textures/PostProcess_texture.tga")
	{
		//Creo los modelos de la escena y los añado vector de modelos a dibujar.
		//El constructor de estos modelos recibe tanto una posición, rotación y translación inicial,
		//así como un modelo 3d y una textura opcional.

		//Modelo de satélite (artificial)
		Scene::drawableModels.push_back(new Model(
			glm::vec3( 0.00f, 0.20f, 1.30f ), 
			glm::vec3( 0.02f, 0.02f, 0.02f ), 
			glm::vec3( 75.0f, 0.00f, 0.00f ),
			"../../assets/models/satellite.obj" ));

		//Modelo de planeta
		Scene::drawableModels.push_back(new Model(
			glm::vec3( 0.00f, 0.00f, -70.0f  ), 
			glm::vec3( 30.0f, 30.0f,  30.0f  ), 
			glm::vec3( 0.00f, 1.00f,  0.00f  ),
			"../../assets/models/planet2.obj", 
			"../../assets/textures/mercuryHD.tga"));

		//Modelo de luna
		Scene::drawableModels.push_back(new Model(
			glm::vec3(-1.20f, -0.50f, 0.00f),
			glm::vec3( 0.20f,  0.20f, 0.20f),
			glm::vec3( 0.00f,  1.00f, 0.00f),
			"../../assets/models/planet2.obj",
			"../../assets/textures/iceHD.tga"));

		//Creo una jerarquía de escena, estableciedo como padre del modelo "satelite" y "luna" 
		//el planeta principal
		drawableModels[0]->SetParent(this->drawableModels[1]);
		drawableModels[2]->SetParent(this->drawableModels[1]);

		//Ajusto la transparencia de la luna al 50%
		drawableModels[2]->SetTransparency(0.50f);

		//Calculo el tamaño de la ventana ( projection ) y ajusto las variables que se usarán para
		//la rotación de la cámara
		resize(width, height);
		camera.camera_angleX = camera.camera_deltaX = 0.0;
		camera.camera_angleY = camera.camera_deltaY = 0.0;
		camera.mouse_press = false;
	}

	void Scene::update()
	{
		//Variables acumulativas para la rotación de los modelos
		accumulated_rotation1 += 0.5f;
		accumulated_rotation2 += 0.025f;

		//Ajusto la rotación de cada modelo cada update
		drawableModels[0]->SetRotation(glm::vec3(drawableModels[0]->rotation.x, accumulated_rotation1,     drawableModels[0]->rotation.z));
		drawableModels[1]->SetRotation(glm::vec3(drawableModels[1]->rotation.x, accumulated_rotation2,     drawableModels[1]->rotation.z));
		drawableModels[2]->SetRotation(glm::vec3(drawableModels[1]->rotation.x, -accumulated_rotation2 *2, drawableModels[1]->rotation.z));

		//Actualizo las propiedades de la cámara cada update
		camera.set_target (camera.get_location().x, camera.get_location().y, camera.get_location().z - 1.f);
		camera.rotate     (camera.calculate_camera_rotation());
		camera.move       (cameraPos);
	}

	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Renderizo el skybox
		skybox.render(camera);

		//Recorro los renderers de cada modelo dentro de drawableModels
		for (auto &model : drawableModels)
		{
			model->render(camera);
		}

		//Renderizo el postproceso
		postprocess.render(camera);
	}

	void Scene::resize(int _width, int _height)
	{
		//Ajusto la nueva projection
		scene_width  = _width;
		scene_height = _height;
		camera.set_camera_aspect_ratio(float(scene_width) / scene_height);

		glViewport(0, 0, scene_width, scene_height);
	}

	void Scene::on_key_pressed(int key_code)
	{
		//Variables con la posición y rotación de la cámara en la escena
		glm::vec3 lookatPosition;
		glm::vec3 cameraPosition;

		//Cálculo de los vectores de la cámara
		glm::vec3 forwardVector(camera.get_model_view()[0][2], camera.get_model_view()[1][2], camera.get_model_view()[2][2]);
		glm::vec3 leftVector   (camera.get_model_view()[0][0], camera.get_model_view()[1][0], camera.get_model_view()[2][0]);
		glm::vec3 upVector     (0, 1, 0);

		//Variable con la velocidad de desplazamiento
		camera_movementSpeed = 0.1f;

		switch (key_code)
		{
			//W: Movimiento de la cámara en dirección FORWARD
		case sf::Keyboard::W:
			cameraPos      = -forwardVector * camera_movementSpeed;
			lookatPosition =  forwardVector * camera_movementSpeed;
			break;

			//A: Movimiento en dirección -LEFTVECTOR
		case sf::Keyboard::A:
			cameraPos      = -leftVector * camera_movementSpeed;
			lookatPosition =  leftVector * camera_movementSpeed;
			break;

			//A: Movimiento en dirección FORWARDVECTOR
		case sf::Keyboard::S:
			cameraPos      = forwardVector * camera_movementSpeed;
			lookatPosition = forwardVector * camera_movementSpeed;
			break;

			//A: Movimiento en dirección LEFTVECTOR
		case sf::Keyboard::D:
			cameraPos      = leftVector    * camera_movementSpeed;
			lookatPosition = forwardVector * camera_movementSpeed;
			break;
		}
	}

	//Si se suelta la tecla de desplazamiento, se detiene toda translación
	void Scene::on_key_release(int key_code)
	{ 
		cameraPos = glm::vec3(0.f, 0.f, 0.f); 
	}

	//Al arrastrar el mouse, si a la vez se pulsa el mouse, se rota la cámara
	void Scene::on_mouse_drag(int pointer_x, int pointer_y)
	{
		//si se pulsa y mueve, actualizamos la variación del desplazamient
		if (camera.mouse_press){
			camera.camera_deltaX = 0.025f * float(camera.prev_mouseY - pointer_y) / float(scene_height);
			camera.camera_deltaY = 0.025f * float(camera.prev_mouseX - pointer_x) / float(scene_width);
		}
	}

	//Si se pulsa el ratón, guardamos la posición del mouse y habilitamos la variable de pulsación
	void Scene::on_mouse_press(int mouse_coord_x, int mouse_coord_y, bool down)
	{
		if ((camera.mouse_press = down) == true){
			camera.prev_mouseX = mouse_coord_x;
			camera.prev_mouseY = mouse_coord_y;
		}

		//Sin pulsación, se reinicia el delta
		else{
			camera.camera_deltaX = camera.camera_deltaY = 0.0; 
		}
	}
}