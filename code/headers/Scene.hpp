/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <vector>
#include <GL/glew.h>            // Debe incluirse antes que gl.h

#include "Camera.hpp"
#include "Skybox.hpp"
#include "Model.hpp"
#include "PostProcess.hpp"

#include <SFML/Window.hpp>
#include <string>

namespace E2_3DA
{
	class Scene
	{
	private:

		Camera camera; //Cámara única de la escena
		Skybox skybox; //Skybox único de la escena 
		PostProcess postprocess; //Postproceso único en la escena
		
		glm::vec3 cameraPos;	    //Posición de la cámara en la escena
		float camera_movementSpeed; //Variable multiplicadora de la velocidad de desplazamiento de la cámara

		std::vector<Model*> drawableModels; //Vector con modelos a dibujar en la escena
		
		float accumulated_rotation1; //Float que posee la rotación acumulada de modelos en la escena
		float accumulated_rotation2; //Float que posee la rotación acumulada de modelos en la escena

		int scene_width;  //Ancho de la ventana
		int scene_height; //Alto de la ventana

	public:

		Scene(int width, int height);

		void update();
		void render();

		void resize(int width, int height);

		void on_mouse_press    (int pointer_x, int pointer_y, bool down);
		void on_mouse_drag     (int pointer_x, int pointer_y);             
		void on_key_pressed    (int key_code);
		void on_key_release    (int key_code);
	};
}

#endif