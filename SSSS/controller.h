#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"  // Incluye la definici�n de la clase Snake para manejar las direcciones y el estado de la serpiente

// Clase que gestiona la entrada del usuario y las direcciones de la serpiente
class Controller {
 public:
  // M�todo para manejar la entrada del usuario y actualizar la direcci�n de la serpiente
  void HandleInput(bool &running, Snake &snake) const;

 private:
  // M�todo auxiliar para cambiar la direcci�n de la serpiente
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif

