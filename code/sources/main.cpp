/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#include <assert.h>
#include <GL/glew.h>

#include "Scene.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace E2_3DA;

int main()
{
	//Constantes de la proporcion de pantalla a 1280x720(16:9)
	const int WINDOW_WIDTH  = 1280;
	const int WINDOW_HEIGHT = 720;

	// Se crea la ventana de SFML, que es donde se creará el contexto de OpenGL:
	Window window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3D Avanzado - Práctica 2", Style::Default, ContextSettings(32));

	// Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:
	GLenum glew_initialization = glewInit();
	assert(glew_initialization == GLEW_OK);

	//Habilitamos propiedades OpenGL
	glEnable   (GL_CULL_FACE );
	glEnable   (GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	window.setVerticalSyncEnabled(true);

	//Creamos una escena
	Scene scene(WINDOW_WIDTH, WINDOW_HEIGHT);

	bool running = true;

	do
	{
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				//Al cerrar la ventana se detiene la ejecución
				case Event::Closed:
				{
					running = false;
					break;
				}

				//Al reescalar la ventana se recalcula el tamaño de la vista y la matriz de proyeccion
				case Event::Resized:
				{
					Vector2u window_size = window.getSize();
					scene.resize(window_size.x, window_size.y);

					break;
				}

				//Al pulsar una tecla (WASD) Se mueve la cámara
				case Event::KeyPressed:
				{
					scene.on_key_pressed(event.key.code);
					break;
				}

				//Si se suelta esa tecla, se detiene el desplazamiento
				case Event::KeyReleased:
				{
					scene.on_key_release(event.key.code);
					break;
				}

				//Al pulsar el mouse (Y moverlo) se cambia la rotación de la cámara
				case Event::MouseButtonPressed:
				{
					scene.on_mouse_press(event.mouseButton.x, event.mouseButton.y, true);
					break;
				}

				//Si se suelta el mouse, se reinicia la variación de posiciones del mouse y el desplazamiento
				case Event::MouseButtonReleased:
				{
					scene.on_mouse_press(event.mouseButton.x, event.mouseButton.y, false);
					break;
				}

				//Al moverse el mouse, se actualiza el delta de la posicion del ratón
				case Event::MouseMoved:
				{
					scene.on_mouse_drag(event.mouseMove.x, event.mouseMove.y);
					break;
				}
			}
		}

		scene.update();

		scene.render();

		window.display();

	} while (running);

	return (EXIT_SUCCESS);
}