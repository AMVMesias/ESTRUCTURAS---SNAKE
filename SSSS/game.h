#ifndef GAME_H
#define GAME_H

#include <random>      // Incluye funciones para generaci�n de n�meros aleatorios
#include <SDL2/SDL.h> // Incluye las definiciones de SDL
#include "controller.h" // Incluye la definici�n de la clase Controller
#include "renderer.h"   // Incluye la definici�n de la clase Renderer
#include "snake.h"       // Incluye la definici�n de la clase Snake

// Clase que gestiona la l�gica del juego
class Game {
 public:
  // Constructor que inicializa el juego con el tama�o de la cuadr�cula
  Game(std::size_t grid_width, std::size_t grid_height);

  // M�todo para ejecutar el juego
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

  // M�todo para obtener el puntaje actual
  int GetScore() const;

  // M�todo para obtener el tama�o actual de la serpiente
  int GetSize() const;

 private:
  // Objeto de la clase Snake que representa la serpiente
  Snake snake;

  // Estructura para representar la comida en el juego
  SDL_Point food;

  // Generador de n�meros aleatorios y distribuciones
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w; // Distribuci�n para la posici�n x de la comida
  std::uniform_int_distribution<int> random_h; // Distribuci�n para la posici�n y de la comida

  // Puntaje del juego
  int score{0};

  // M�todo para colocar la comida en una posici�n aleatoria
  void PlaceFood();

  // M�todo para actualizar el estado del juego
  void Update();
};

#endif

