#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h> // Incluir SDL_mixer
#include <vector>
#include <cstdlib>
#include <string> // Para manejar cadenas de texto
#include <iostream>

const int CELL_SIZE = 20;
 // Asegúrate de definir la altura de la pantalla

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

class Game {
public:
    Game(SDL_Renderer* renderer)
        : renderer(renderer), isRunning(true), direction(RIGHT), score(0), music(nullptr), eatSound(nullptr) {
        snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        generateWalls();
        spawnFood();
        if (!initMusic() || !initSound()) {
            isRunning = false;
        }
    }

    ~Game() {
        cleanupMusic();
        cleanupSound();
    }

    void start(int difficulty) {
        while (isRunning) {
            handleEvents();
            update();
            render();
            SDL_Delay(1000 / (10 + difficulty * 5)); // Ajustar velocidad según dificultad
        }
    }

    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_w:
                        if (direction != DOWN) direction = UP;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        if (direction != UP) direction = DOWN;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        if (direction != RIGHT) direction = LEFT;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (direction != LEFT) direction = RIGHT;
                        break;
                }
            }
        }
    }

    void update() {
        Point newHead = snake.front();
        switch (direction) {
            case UP:    newHead.y -= CELL_SIZE; break;
            case DOWN:  newHead.y += CELL_SIZE; break;
            case LEFT:  newHead.x -= CELL_SIZE; break;
            case RIGHT: newHead.x += CELL_SIZE; break;
        }

        if (checkCollision(newHead)) {
            isRunning = false;
            return;
        }

        snake.insert(snake.begin(), newHead);

        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            spawnFood();
            Mix_PlayChannel(-1, eatSound, 0); // Reproducir el efecto de sonido de comida
        } else {
            snake.pop_back();
        }
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fondo negro
        SDL_RenderClear(renderer);

        // Renderizar comida
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Comida roja
        SDL_Rect foodRect = { food.x, food.y, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(renderer, &foodRect);

        // Renderizar serpiente
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Serpiente verde
        for (const auto& segment : snake) {
            SDL_Rect snakeRect = { segment.x, segment.y, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &snakeRect);
        }

        // Renderizar paredes
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Paredes grises
        for (const auto& wall : walls) {
            SDL_Rect wallRect = { wall.x, wall.y, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &wallRect);
        }

        SDL_RenderPresent(renderer);
    }

private:
    SDL_Renderer* renderer;
    bool isRunning;
    Direction direction;
    std::vector<Point> snake;
    std::vector<Point> walls;
    Point food;
    int score;
    Mix_Music* music;
    Mix_Chunk* eatSound; // Efecto de sonido

    bool initMusic() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            return false;
        }
        music = Mix_LoadMUS("assets/music/musica1.mp3");
        if (!music) {
            return false;
        }
        Mix_PlayMusic(music, -1); // Reproducir música en bucle
        return true;
    }

    bool initSound() {
        eatSound = Mix_LoadWAV("assets/music/sound.wav");
        if (!eatSound) {
            std::cerr << "Error al cargar el sonido de comida: " << Mix_GetError() << std::endl;
            return false;
        }
        return true;
    }

    void cleanupMusic() {
        if (music) {
            Mix_FreeMusic(music);
        }
        Mix_CloseAudio();
    }

    void cleanupSound() {
        if (eatSound) {
            Mix_FreeChunk(eatSound);
        }
    }

    void spawnFood() {
        do {
            food.x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
            food.y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
        } while (checkCollision(food)); // Asegurarse de que la comida no aparezca en la serpiente o en las paredes
    }

    void generateWalls() {
        // Crear un borde alrededor del área de juego
        for (int x = 0; x < SCREEN_WIDTH; x += CELL_SIZE) {
            walls.push_back({x, 0});
            walls.push_back({x, SCREEN_HEIGHT - CELL_SIZE});
        }
        for (int y = 0; y < SCREEN_HEIGHT; y += CELL_SIZE) {
            walls.push_back({0, y});
            walls.push_back({SCREEN_WIDTH - CELL_SIZE, y});
        }

        // Puedes agregar más paredes aquí si lo deseas
    }

    bool checkCollision(const Point& p) {
        // Colisión con los bordes
        if (p.x < 0 || p.x >= SCREEN_WIDTH || p.y < 0 || p.y >= SCREEN_HEIGHT) {
            return true;
        }

        // Colisión con la serpiente misma
        for (size_t i = 1; i < snake.size(); ++i) {
            if (p.x == snake[i].x && p.y == snake[i].y) {
                return true;
            }
        }

        // Colisión con las paredes
        for (const auto& wall : walls) {
            if (p.x == wall.x && p.y == wall.y) {
                return true;
            }
        }

        return false;
    }
};

#endif // SNAKE_GAME_H
