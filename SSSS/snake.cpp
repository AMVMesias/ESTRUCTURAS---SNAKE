#include "snake.h"        // Incluye la definición de la clase Snake
#include <cmath>          // Para funciones matemáticas como fmod
#include <iostream>       // Para operaciones de entrada/salida estándar

// Actualiza la posición de la serpiente
void Snake::Update() {
  // Captura la celda actual de la cabeza antes de actualizar
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

  // Actualiza la cabeza de la serpiente
  UpdateHead();

  // Captura la celda actual de la cabeza después de actualizar
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

  // Actualiza todos los segmentos del cuerpo si la cabeza se ha movido a una nueva celda
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

// Actualiza la posición de la cabeza de la serpiente según la dirección
void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;   // Mueve la cabeza hacia arriba
      break;

    case Direction::kDown:
      head_y += speed;   // Mueve la cabeza hacia abajo
      break;

    case Direction::kLeft:
      head_x -= speed;   // Mueve la cabeza hacia la izquierda
      break;

    case Direction::kRight:
      head_x += speed;   // Mueve la cabeza hacia la derecha
      break;
  }

  // Envuelve la cabeza de la serpiente a través de los bordes del juego si se sale de la pantalla
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

// Actualiza el cuerpo de la serpiente según el movimiento de la cabeza
void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Añade la ubicación anterior de la cabeza al vector del cuerpo
  body.push_back(prev_head_cell);

  if (!growing) {
    // Elimina la cola del vector si no está creciendo
    body.erase(body.begin());
  } else {
    growing = false;  // Restablece el estado de crecimiento
    size++;  // Aumenta el tamaño de la serpiente
  }

  // Verifica si la serpiente ha chocado con su propio cuerpo
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;  // La serpiente muere si choca con su propio cuerpo
    }
  }
}

// Marca la serpiente para crecer en la próxima actualización
void Snake::GrowBody() { growing = true; }

// Verifica si una celda está ocupada por la serpiente
bool Snake::SnakeCell(int x, int y) {
  // Verifica si la celda coincide con la cabeza de la serpiente
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  // Verifica si la celda coincide con cualquier segmento del cuerpo de la serpiente
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;  // La celda no está ocupada por la serpiente
}

