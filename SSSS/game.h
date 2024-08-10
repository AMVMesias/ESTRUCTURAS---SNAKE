#ifndef GAME_H
#define GAME_H

#include <random>      // Incluye funciones para generación de números aleatorios
#include <SDL2/SDL.h> // Incluye las definiciones de SDL
#include "controller.h" // Incluye la definición de la clase Controller
#include "renderer.h"   // Incluye la definición de la clase Renderer
#include "snake.h"       // Incluye la definición de la clase Snake

// Clase que gestiona la lógica del juego
class Game {
 public:
  // Constructor que inicializa el juego con el tamaño de la cuadrícula
  Game(std::size_t grid_width, std::size_t grid_height);

  // Método para ejecutar el juego
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

  // Método para obtener el puntaje actual
  int GetScore() const;

  // Método para obtener el tamaño actual de la serpiente
  int GetSize() const;

 private:
  // Objeto de la clase Snake que representa la serpiente
  Snake snake;

  // Estructura para representar la comida en el juego
  SDL_Point food;

  // Generador de números aleatorios y distribuciones
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w; // Distribución para la posición x de la comida
  std::uniform_int_distribution<int> random_h; // Distribución para la posición y de la comida

  // Puntaje del juego
  int score{0};

  // Método para colocar la comida en una posición aleatoria
  void PlaceFood();

  // Método para actualizar el estado del juego
  void Update();
};

#endif

