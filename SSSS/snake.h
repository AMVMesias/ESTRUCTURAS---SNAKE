#ifndef SNAKE_H
#define SNAKE_H

#include <vector>      // Incluye la definici�n de la clase std::vector
#include <SDL2/SDL.h> // Incluye las definiciones de SDL

// Clase que representa la serpiente en el juego
class Snake {
 public:
  // Enum para representar las direcciones posibles de la serpiente
  enum class Direction { kUp, kDown, kLeft, kRight };

  // Constructor que inicializa la serpiente en el centro de la cuadr�cula
  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  // M�todo para actualizar el estado de la serpiente
  void Update();

  // M�todo para hacer crecer el cuerpo de la serpiente
  void GrowBody();

  // M�todo para verificar si una celda est� ocupada por la serpiente
  bool SnakeCell(int x, int y);

  // Direcci�n actual de la serpiente
  Direction direction = Direction::kUp;

  // Velocidad de la serpiente
  float speed{0.1f};

  // Tama�o de la serpiente
  int size{1};

  // Estado de la serpiente (si est� viva o no)
  bool alive{true};

  // Posici�n actual de la cabeza de la serpiente
  float head_x;
  float head_y;

  // Vector que almacena las posiciones del cuerpo de la serpiente
  std::vector<SDL_Point> body;

 private:
  // M�todo para actualizar la posici�n de la cabeza de la serpiente
  void UpdateHead();

  // M�todo para actualizar las posiciones del cuerpo de la serpiente
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  // Indica si la serpiente est� creciendo
  bool growing{false};

  // Dimensiones de la cuadr�cula del juego
  int grid_width;
  int grid_height;
};

#endif
