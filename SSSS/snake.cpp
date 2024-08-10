#include "snake.h"        // Incluye la definici�n de la clase Snake
#include <cmath>          // Para funciones matem�ticas como fmod
#include <iostream>       // Para operaciones de entrada/salida est�ndar

// Actualiza la posici�n de la serpiente
void Snake::Update() {
  // Captura la celda actual de la cabeza antes de actualizar
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

  // Actualiza la cabeza de la serpiente
  UpdateHead();

  // Captura la celda actual de la cabeza despu�s de actualizar
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};

  // Actualiza todos los segmentos del cuerpo si la cabeza se ha movido a una nueva celda
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

// Actualiza la posici�n de la cabeza de la serpiente seg�n la direcci�n
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

  // Envuelve la cabeza de la serpiente a trav�s de los bordes del juego si se sale de la pantalla
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

// Actualiza el cuerpo de la serpiente seg�n el movimiento de la cabeza
void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // A�ade la ubicaci�n anterior de la cabeza al vector del cuerpo
  body.push_back(prev_head_cell);

  if (!growing) {
    // Elimina la cola del vector si no est� creciendo
    body.erase(body.begin());
  } else {
    growing = false;  // Restablece el estado de crecimiento
    size++;  // Aumenta el tama�o de la serpiente
  }

  // Verifica si la serpiente ha chocado con su propio cuerpo
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;  // La serpiente muere si choca con su propio cuerpo
    }
  }
}

// Marca la serpiente para crecer en la pr�xima actualizaci�n
void Snake::GrowBody() { growing = true; }

// Verifica si una celda est� ocupada por la serpiente
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
  return false;  // La celda no est� ocupada por la serpiente
}

