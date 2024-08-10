#include <iostream>          // Para operaciones de entrada/salida como std::cout
#include "controller.h"     // Para la clase Controller que maneja la entrada del usuario
#include "game.h"           // Para la clase Game que maneja la lógica del juego
#include "renderer.h"       // Para la clase Renderer que maneja el dibujo en pantalla
#include <SDL2/SDL.h>       // Para funciones y tipos de la biblioteca SDL

// Función principal del programa
int SDL_main(int argc, char* argv[]) {
  // Constantes de configuración
  constexpr std::size_t kFramesPerSecond{60};  // Número de frames por segundo
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};  // Duración de cada frame en milisegundos
  constexpr std::size_t kScreenWidth{640};  // Ancho de la ventana en píxeles
  constexpr std::size_t kScreenHeight{640}; // Alto de la ventana en píxeles
  constexpr std::size_t kGridWidth{32};     // Número de columnas en el grid
  constexpr std::size_t kGridHeight{32};    // Número de filas en el grid

  // Inicializa los objetos principales
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);  // Crea el renderer con el tamaño de pantalla y grid
  Controller controller;  // Crea el controlador para manejar la entrada del usuario
  Game game(kGridWidth, kGridHeight);  // Crea el juego con el tamaño del grid

  // Ejecuta el juego con el controlador, renderer y duración del frame
  game.Run(controller, renderer, kMsPerFrame);

  // Mensajes al finalizar el juego
  std::cout << "Game has terminated successfully!\n";  // Mensaje de éxito
  std::cout << "Score: " << game.GetScore() << "\n";  // Muestra el puntaje final
  std::cout << "Size: " << game.GetSize() << "\n";    // Muestra el tamaño final de la serpiente

  return 0;  // Retorna 0 para indicar que el programa terminó correctamente
}

