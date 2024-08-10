#ifndef SNAKE_H
#define SNAKE_H

#include <vector>      // Incluye la definición de la clase std::vector
#include <SDL2/SDL.h> // Incluye las definiciones de SDL

// Clase que representa la serpiente en el juego
class Snake {
 public:
  // Enum para representar las direcciones posibles de la serpiente
  enum class Direction { kUp, kDown, kLeft, kRight };

  // Constructor que inicializa la serpiente en el centro de la cuadrícula
  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  // Método para actualizar el estado de la serpiente
  void Update();

  // Método para hacer crecer el cuerpo de la serpiente
  void GrowBody();

  // Método para verificar si una celda está ocupada por la serpiente
  bool SnakeCell(int x, int y);

  // Dirección actual de la serpiente
  Direction direction = Direction::kUp;

  // Velocidad de la serpiente
  float speed{0.1f};

  // Tamaño de la serpiente
  int size{1};

  // Estado de la serpiente (si está viva o no)
  bool alive{true};

  // Posición actual de la cabeza de la serpiente
  float head_x;
  float head_y;

  // Vector que almacena las posiciones del cuerpo de la serpiente
  std::vector<SDL_Point> body;

 private:
  // Método para actualizar la posición de la cabeza de la serpiente
  void UpdateHead();

  // Método para actualizar las posiciones del cuerpo de la serpiente
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  // Indica si la serpiente está creciendo
  bool growing{false};

  // Dimensiones de la cuadrícula del juego
  int grid_width;
  int grid_height;
};

#endif
