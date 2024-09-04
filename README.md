# ESTRUCTURA DE DATOS 

## Snake Game

### Descripción
Este programa es un implementación del clásico juego de la Serpiente, desarrollado utilizando  SDL2(Simple DirectMedia Layer 2) en C++. Ofrece una experiencia del juego con gráficos orientados al pixelart, efectos de sonido y características adicionales.

### Características Principales

- Jugabilidad clásica de la Serpiente 
- Múltiples niveles de dificultad
- Música de fondo y efectos de sonido
- Sistema de puntuación alta con almacenamiento persistente
- Menú de pausa y pantalla de fin de juego
- Nivele progresivamente desafiantes

### Visión General del Juego

[![Captura-de-pantalla-2024-09-03-194827.png](https://i.postimg.cc/y8kTMDGq/Captura-de-pantalla-2024-09-03-194827.png)](https://postimg.cc/TKvmm3GQ)

El juego comienza con un menú principal donde los jugadores pueden elegir nivel de dificultad, ver puntuaciones altas o leer instrucciones. Una vez que comienza el juego, los jugadores controlan una serpiente que crece más larga a medida que come alimentos. El objetivo es comer la mayor cantidad de comida posible sin chocar con las paredes o el propio cuerpo de la serpiente.

### Historia del Juego de la Serpiente

El juego de la Serpiente es uno de los primeros juegos digitales, apareciendo por primera vez en la década de 1970. Ganó popularidad generalizada a finales de los años 90 cuando venía precargado en los teléfonos móviles Nokia. La simplicidad del juego y su naturaleza adictiva lo convirtieron en un fenómeno cultural.

[![Juego-Clasico.png](https://i.postimg.cc/FRQgkDHm/Juego-Clasico.png)](https://postimg.cc/wt0sS5Nb)

Nuestra versión rinde homenaje a este clásico mientras introduce elementos modernos para mejorar la experiencia de juego.

### Detalles Técnicos

El juego esta construido utilizando lo siguiente:

- Lenguaje de programación C++
- SDL2 para renderizado de gráficos y manejo de entrada
- SDL_mixer para reproducción de audio
- SDL_ttf para renderizado de texto
- SDL_image para carga de imágenes
- nlohmann/json para análisis de JSON y almacenamiento de puntuaciones altas

### Estructura del Juego

El juego está estructurado en varios componentes clave:

1. **Engine**: El motor central del juego que inicializa SDL y gestiona el bucle del juego.
2. **Menu**: Maneja el menú principal, la selección de dificultad y la pantalla de instrucciones.
3. **Game**: Gestiona el juego real, incluyendo el movimiento de la serpiente, detección de colisiones y progresión de niveles.
4. **Score**: Maneja el sistema de puntuación, incluyendo la carga y guardado en un archivo JSON.

### Cómo Jugar

1. Usa las teclas de flecha o WASD para controlar la dirección de la serpiente.
2. Come alimentos para crecer más y aumentar tu puntuación.
3. Evita chocar con las paredes y el propio cuerpo de la serpiente.
4. Progresa a través de niveles cada vez más difíciles.
5. ¡Intenta lograr la puntuación más alta posible!

### Compilación y Ejecución

(Descrito en los archivos PDF incluidos)

### Mejoras Futuras

- Incluir Multijugador
- Skins personalizables para la serpiente
- Power-ups, alimentos especiales, bombas
- Diseño de niveles más diversos


¡Disfruta jugando nuestra versión del clásico juego de la Serpiente!

Creado por: Mesias Mariscal \\
Leonel Tipan \\ 
Juan Quimbiulco \\ 
Alison Miranda
