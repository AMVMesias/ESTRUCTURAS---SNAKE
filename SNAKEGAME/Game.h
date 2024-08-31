#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>    // Para renderizar texto
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include "Score.h"

const int CELL_SIZE = 20;  // Tamaño de cada celda del juego

enum Direction { UP, DOWN, LEFT, RIGHT };
enum GameLevel { LEVEL_1, LEVEL_2, LEVEL_3 };
enum GameState { RUNNING, PAUSED, MENU };
enum MenuOption { RESUME, MAIN_MENU, EXIT };


struct Point {
    int x, y;  // Coordenadas
};

class Game {
public:
    Game(SDL_Renderer* renderer)
        : renderer(renderer), isRunning(true), direction(RIGHT), score(0), music(nullptr), eatSound(nullptr), currentLevel(LEVEL_1), scoreBoard(renderer),  gameState(MENU), selectedOption(RESUME)  {
        snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        generarParedes();  // Generar paredes del borde
        generarComida();  // Generar la primera comida

        if (!inicializarMusica() || !inicializarSonido()) {
            isRunning = false;
        }
        startTime = std::chrono::steady_clock::now();  // Guardar el tiempo de inicio
    }

    ~Game() {
        limpiar();
    }

    void iniciar(int difficulty) {
        gameState = RUNNING;
        while (isRunning) {
            manejarEventos();
            if (gameState == RUNNING) {
                actualizar();
            }
            renderizar();
            SDL_Delay(1000 / (10 + difficulty * 5));

            if (gameState == RUNNING) {
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
                if (elapsedTime >= 4 && currentLevel == LEVEL_1) {
                    nivel2();
                } else if (elapsedTime >= 15 && currentLevel == LEVEL_2) {
                    nivel3();
                }
            }
        }
    }


    void manejarEventos() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                SDL_Quit();
                exit(0);
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (gameState == RUNNING && direction != DOWN) {
                            direction = UP;
                        } else if (gameState == PAUSED || gameState == MENU) {
                            selectedOption = (MenuOption)((selectedOption - 1 + 3) % 3);
                        }
                        break;
                    case SDLK_DOWN:
                        if (gameState == RUNNING && direction != UP) {
                            direction = DOWN;
                        } else if (gameState == PAUSED || gameState == MENU) {
                            selectedOption = (MenuOption)((selectedOption + 1) % 3);
                        }
                        break;
                    case SDLK_LEFT:
                        if (gameState == RUNNING && direction != RIGHT) direction = LEFT;
                        break;
                    case SDLK_RIGHT:
                        if (gameState == RUNNING && direction != LEFT) direction = RIGHT;
                        break;
                    case SDLK_ESCAPE:
                    case SDLK_p:
                        if (gameState == RUNNING) {
                            gameState = PAUSED;
                            selectedOption = RESUME;
                        } else if (gameState == PAUSED) {
                            gameState = RUNNING;
                        }
                        break;
                    case SDLK_RETURN:
                        if (gameState == PAUSED || gameState == MENU) {
                            executeMenuOption();
                        }
                        break;
                }
            }
        }
    }

    void executeMenuOption() {
        switch (selectedOption) {
            case RESUME:
                gameState = RUNNING;
                break;
            case MAIN_MENU:
                resetGame();
                gameState = MENU;
                break;
            case EXIT:
                isRunning = false;
                exit(0);
                break;
        }
    }

    void resetGame() {
        snake.clear();
        snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        direction = RIGHT;
        score = 0;
        currentLevel = LEVEL_1;
        walls.clear();
        generarParedes();
        generarComida();
        startTime = std::chrono::steady_clock::now();
    }

    void actualizar() {
        if (gameState == PAUSED) {
            return; // No actualizar si el juego está pausado
        }
        Point newHead = snake.front();
        switch (direction) {
            case UP:    newHead.y -= CELL_SIZE; break;
            case DOWN:  newHead.y += CELL_SIZE; break;
            case LEFT:  newHead.x -= CELL_SIZE; break;
            case RIGHT: newHead.x += CELL_SIZE; break;
        }

        if (verificarColision(newHead)) {
            isRunning = false;
            return;
        }

        snake.insert(snake.begin(), newHead);

        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            scoreBoard.addScore({ "Jugador", score, (int)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() });
            generarComida();
            Mix_PlayChannel(-1, eatSound, 0);
        } else {
            snake.pop_back();
        }
    }

    void renderizar() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (gameState == RUNNING || gameState == PAUSED) {
            // Renderizar comida
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect foodRect = { food.x, food.y, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &foodRect);

            // Renderizar serpiente
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for (const auto& segment : snake) {
                SDL_Rect snakeRect = { segment.x, segment.y, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &snakeRect);
            }

            // Renderizar paredes
            SDL_SetRenderDrawColor(renderer, currentLevel == LEVEL_1 ? 128 : (currentLevel == LEVEL_2 ? 64 : 32), 128, 128, 255);
            for (const auto& wall : walls) {
                SDL_Rect wallRect = { wall.x, wall.y, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &wallRect);
            }

            // Renderizar puntuación en pantalla
            SDL_Color white = {255, 255, 255, 255};
            scoreBoard.renderText(("Score: " + std::to_string(score)).c_str(), 9, 9, white);

            if (gameState == PAUSED) {
                renderMenu("PAUSED");
            }
        } else if (gameState == MENU) {
            Menu menu(renderer);
            menu.render();
        }

        SDL_RenderPresent(renderer);
    }


    void renderMenu(const std::string& title) {
        // Semi-transparent overlay
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Color white = {255, 255, 255, 255};
        SDL_Color selected = {0, 255, 0, 255};

        // Render title
        scoreBoard.renderText(title.c_str(), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, selected);

        // Render menu options
        scoreBoard.renderText("Resume", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, selectedOption == RESUME ? selected : white);
        scoreBoard.renderText("Main Menu", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 50, selectedOption == MAIN_MENU ? selected : white);
        scoreBoard.renderText("EXIT", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 100, selectedOption == EXIT ? selected : white);
    }

    void nivel2() {
        mostrarPantallaTransicion("¡Superaste el primer nivel!", "¿Quieres continuar al Nivel 2?", LEVEL_2);
    }

    void nivel3() {
        mostrarPantallaTransicion("¡Superaste el Nivel 2!", "¿Quieres continuar al Nivel 3?", LEVEL_3);
    }

private:
    bool inicializarMusica() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            return false;
        }
        music = Mix_LoadMUS("assets/music/musica2.mp3");
        if (!music) {
            return false;
        }
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(30);
        return true;
    }

    bool inicializarSonido() {
        eatSound = Mix_LoadWAV("assets/music/sound.wav");
        if (!eatSound) {
            std::cerr << "Error al cargar el sonido de comida: " << Mix_GetError() << std::endl;
            return false;
        }
        Mix_VolumeChunk(eatSound, 128);
        return true;
    }

    void generarParedes() {
        for (int x = 0; x < SCREEN_WIDTH; x += CELL_SIZE) {
            walls.push_back({x, 0});
            walls.push_back({x, SCREEN_HEIGHT - CELL_SIZE});
        }
        for (int y = 0; y < SCREEN_HEIGHT; y += CELL_SIZE) {
            walls.push_back({0, y});
            walls.push_back({SCREEN_WIDTH - CELL_SIZE, y});
        }
    }

    void generarParedesNivel2() {
        generarParedes();
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;
        for (int i = -5; i <= 5; i++) {
            walls.push_back({centerX + i * CELL_SIZE, centerY});
            walls.push_back({centerX, centerY + i * CELL_SIZE});
        }
    }

    void generarParedesNivel3() {
        generarParedes();
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;
        for (int i = -7; i <= 7; i++) {
            walls.push_back({centerX + i * CELL_SIZE, centerY});
            walls.push_back({centerX, centerY + i * CELL_SIZE});
        }
    }

    void generarComida() {
        do {
            food.x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
            food.y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
        } while (verificarColision(food));
    }

    bool verificarColision(const Point& p) {
        if (p.x < 0 || p.x >= SCREEN_WIDTH || p.y < 0 || p.y >= SCREEN_HEIGHT) {
            return true;
        }
        for (size_t i = 1; i < snake.size(); ++i) {
            if (p.x == snake[i].x && p.y == snake[i].y) {
                return true;
            }
        }
        for (const auto& wall : walls) {
            if (p.x == wall.x && p.y == wall.y) {
                return true;
            }
        }
        return false;
    }

    void posicionarSerpienteEnLugarSeguro() {
        snake.clear();
        Point start;
        do {
            start.x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
            start.y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
        } while (verificarColision(start));

        snake.push_back(start);
        direction = RIGHT;
    }

    void mostrarPantallaTransicion(const std::string& mensaje, const std::string& subMensaje, GameLevel nextLevel) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255, 255, 255, 255};
        scoreBoard.renderText(mensaje.c_str(), SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, white);
        scoreBoard.renderText(subMensaje.c_str(), SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, white);
        scoreBoard.renderText("Presiona 'Y' para continuar o 'N' para regresar al menu.", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 + 50, white);
        SDL_RenderPresent(renderer);

        bool continuar = esperarDecisionJugador();
        if (!continuar) {
            isRunning = false;
            return;
        }

        currentLevel = nextLevel;
        if (nextLevel == LEVEL_2) {
            walls.clear();
            generarParedesNivel2();
        } else if (nextLevel == LEVEL_3) {
            walls.clear();
            generarParedesNivel3();
        }
        posicionarSerpienteEnLugarSeguro();
        generarComida();
    }

    bool esperarDecisionJugador() {
        SDL_Event event;
        while (true) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    isRunning = false;
                    return false;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_y) {
                        return true;
                    } else if (event.key.keysym.sym == SDLK_n) {
                        return false;
                    }
                }
            }
        }
    }

    void limpiar() {
        Mix_FreeMusic(music);
        Mix_FreeChunk(eatSound);
        Mix_CloseAudio();
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }

    SDL_Renderer* renderer;
    std::vector<Point> snake;
    std::vector<Point> walls;
    Point food;
    Direction direction;
    bool isRunning;
    int score;
    Mix_Music* music;
    Mix_Chunk* eatSound;
    GameLevel currentLevel;
    Score scoreBoard;
    GameState gameState;
    MenuOption selectedOption;
    std::chrono::steady_clock::time_point startTime;
};

#endif // SNAKE_GAME_H
