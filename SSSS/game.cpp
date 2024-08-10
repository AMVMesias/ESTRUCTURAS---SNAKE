#include "game.h"
#include <iostream>
#include <SDL2/SDL.h>

// Constructor de la clase Game
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),  // Inicializa la serpiente con el tama�o del grid
      engine(dev()),  // Inicializa el generador de n�meros aleatorios
      random_w(0, static_cast<int>(grid_width - 1)),  // Rango para la posici�n x de la comida
      random_h(0, static_cast<int>(grid_height - 1)) {  // Rango para la posici�n y de la comida
  PlaceFood();  // Coloca la comida en una posici�n aleatoria
}

// M�todo principal del juego que ejecuta el bucle del juego
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();  // Marca de tiempo para actualizar el t�tulo de la ventana
  Uint32 frame_start;  // Marca el inicio del frame
  Uint32 frame_end;  // Marca el final del frame
  Uint32 frame_duration;  // Duraci�n del frame
  int frame_count = 0;  // Contador de frames por segundo
  bool running = true;  // Estado del juego

  // Bucle principal del juego
  while (running) {
    frame_start = SDL_GetTicks();  // Captura el inicio del frame

    // Input, Update, Render - el bucle principal del juego
    controller.HandleInput(running, snake);  // Maneja la entrada del teclado
    Update();  // Actualiza el estado del juego
    renderer.Render(snake, food);  // Renderiza la serpiente y la comida

    frame_end = SDL_GetTicks();  // Captura el final del frame

    // Calcula la duraci�n del frame
    frame_count++;
    frame_duration = frame_end - frame_start;

    // Actualiza el t�tulo de la ventana cada segundo
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);  // Actualiza el t�tulo de la ventana con el puntaje y FPS
      frame_count = 0;  // Reinicia el contador de frames
      title_timestamp = frame_end;  // Actualiza la marca de tiempo
    }

    // Ajusta el tiempo de espera para mantener el frame rate deseado
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);  // Retrasa el bucle para mantener la tasa de frames
    }
  }
}

// Coloca la comida en una posici�n aleatoria que no est� ocupada por la serpiente
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);  // Genera una posici�n x aleatoria
    y = random_h(engine);  // Genera una posici�n y aleatoria
    // Verifica que la ubicaci�n no est� ocupada por un segmento de la serpiente
    if (!snake.SnakeCell(x, y)) {
      food.x = x;  // Asigna la posici�n x a la comida
      food.y = y;  // Asigna la posici�n y a la comida
      return;  // Sale del bucle
    }
  }
}

// Actualiza el estado del juego
void Game::Update() {
  if (!snake.alive) return;  // Si la serpiente no est� viva, no hace nada

  snake.Update();  // Actualiza la posici�n de la serpiente

  int new_x = static_cast<int>(snake.head_x);  // Nueva posici�n x de la cabeza de la serpiente
  int new_y = static_cast<int>(snake.head_y);  // Nueva posici�n y de la cabeza de la serpiente

  // Verifica si la comida est� en la misma posici�n que la cabeza de la serpiente
  if (food.x == new_x && food.y == new_y) {
    score++;  // Incrementa el puntaje
    PlaceFood();  // Coloca la comida en una nueva posici�n
    snake.GrowBody();  // Hace crecer la serpiente
    snake.speed += 0.02;  // Aumenta la velocidad de la serpiente
  }
}

// Obtiene el puntaje actual del juego
int Game::GetScore() const { return score; }

// Obtiene el tama�o actual de la serpiente
int Game::GetSize() const { return snake.size; }

