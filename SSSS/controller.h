#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"  // Incluye la definición de la clase Snake para manejar las direcciones y el estado de la serpiente

// Clase que gestiona la entrada del usuario y las direcciones de la serpiente
class Controller {
 public:
  // Método para manejar la entrada del usuario y actualizar la dirección de la serpiente
  void HandleInput(bool &running, Snake &snake) const;

 private:
  // Método auxiliar para cambiar la dirección de la serpiente
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif

