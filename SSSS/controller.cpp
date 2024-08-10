#include "controller.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "snake.h"

// Cambia la direcci�n de la serpiente si la nueva direcci�n no es opuesta a la direcci�n actual
void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  // Cambia la direcci�n de la serpiente solo si no es opuesta a la actual
  // o si la serpiente tiene tama�o 1 (puede girar en cualquier direcci�n)
  if (snake.direction != opposite || snake.size == 1) {
    snake.direction = input;
  }
  return;
}

// Maneja la entrada del teclado y cambia la direcci�n de la serpiente o sale del juego
void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;  // Estructura para eventos de SDL
  // Procesa todos los eventos en la cola
  while (SDL_PollEvent(&e)) {
    // Verifica si el evento es una solicitud de cierre de ventana
    if (e.type == SDL_QUIT) {
      running = false;  // Detiene el bucle del juego
    } else if (e.type == SDL_KEYDOWN) {  // Verifica si se presion� una tecla
      // Cambia la direcci�n de la serpiente seg�n la tecla presionada
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);  // Mueve hacia arriba
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);  // Mueve hacia abajo
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);  // Mueve hacia la izquierda
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);  // Mueve hacia la derecha
          break;
      }
    }
  }
}
