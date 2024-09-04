#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Score.h"

const int CELL_SIZE = 20;
enum GameState { RUNNING, PAUSED, GAME_OVER, MENU, TRANSITION };

enum Direction { UP, DOWN, LEFT, RIGHT };
enum GameLevel { LEVEL_1, LEVEL_2, LEVEL_3 };

struct Point {
    int x, y;
};

class Menu;  // Forward declaration

class Game {
public:
    int levelTime;  // Tiempo restante del nivel en milisegundos

    Game(SDL_Renderer* renderer, Menu* menu)
        : renderer(renderer), menu(menu), direction(RIGHT), score(0), music(nullptr), eatSound(nullptr),
          currentLevel(LEVEL_1), scoreBoard(renderer), gameOver(false), gameState(RUNNING),
          selectedOption(0), gameOverOption(0), isGameStarted(false), difficulty(1),
          headTexture(nullptr), bodyTexture(nullptr), isRunning(true),
          gameTime(0), lastUpdateTime(std::chrono::steady_clock::now()) {
        snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        generarParedes();
        generarComida();

        if (!inicializarMusica() || !inicializarSonido() || !cargarTexturas()) {
            isRunning = false;
        }
    }

    ~Game() {
        limpiar();
    }

    void iniciar(int difficulty) {
        this->difficulty = difficulty;
        isGameStarted = false;
        resetGame();
        while (isRunning) {
            auto currentTime = std::chrono::steady_clock::now();
            auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count();
            lastUpdateTime = currentTime;

            manejarEventos();
            if (isCountingDown) {
                updateCountdown(deltaTime);
            } else if (gameState == RUNNING && isGameStarted) {
                actualizar(deltaTime);
                if (gameState == RUNNING) {
                    gameTime += deltaTime;
                    levelTime -= deltaTime;
                    if (levelTime <= 0) {
                        if (currentLevel == LEVEL_1) {
                            nivel2();
                        } else if (currentLevel == LEVEL_2) {
                            nivel3();
                        } else {
                            handleGameOver();
                        }
                    }
                }
            }
            renderizar();
            SDL_Delay(1000 / (10 + difficulty * 5));
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
                if (gameOver) {
                    handleGameOverInput(event.key.keysym.sym);
                } else if (!isGameStarted) {
                    isGameStarted = true;
                } else {
                    if (gameState == PAUSED) {
                        handlePauseInput(event);
                    } else {
                        switch (event.key.keysym.sym) {
                            case SDLK_UP: case SDLK_w:
                                if (direction != DOWN) direction = UP;
                                break;
                            case SDLK_DOWN: case SDLK_s:
                                if (direction != UP) direction = DOWN;
                                break;
                            case SDLK_LEFT: case SDLK_a:
                                if (direction != RIGHT) direction = LEFT;
                                break;
                            case SDLK_RIGHT: case SDLK_d:
                                if (direction != LEFT) direction = RIGHT;
                                break;
                            case SDLK_ESCAPE:
                                togglePause();
                                break;
                        }
                    }
                }
            } else if (event.type == SDL_TEXTINPUT) {
                if (gameOver && playerName.length() < 20) {
                    playerName += event.text.text;
                }
            }
        }
    }


    void limpiar() {
        Mix_FreeMusic(music);
        Mix_FreeChunk(eatSound);
        Mix_CloseAudio();
        SDL_DestroyTexture(headTexture);
        SDL_DestroyTexture(bodyTexture);
        SDL_DestroyTexture(appleTexture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }

    void nivel2() {
        if (gameOver) return;
        mostrarPantallaTransicion("¡Superaste el primer nivel!", "¿Quieres continuar al Nivel 2?", LEVEL_2);
         levelTime = 90000;  // Reiniciar el tiempo del nivel

    }

    void nivel3() {
        if (gameOver) return;
        mostrarPantallaTransicion("¡Superaste el Nivel 2!", "¿Quieres continuar al Nivel 3?", LEVEL_3);
         levelTime = 90000;  // Reiniciar el tiempo del nivel

    }

private:
    SDL_Renderer* renderer;
    Menu* menu;
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
    bool gameOver;
    GameState gameState;
    int selectedOption;
    int gameOverOption;
    std::string playerName;
    bool isGameStarted;
    int difficulty;
    SDL_Texture* headTexture;
    SDL_Texture* bodyTexture;
    SDL_Texture* appleTexture;
    SDL_Texture* backgroundTexture;
    long long gameTime;
    std::chrono::steady_clock::time_point lastUpdateTime;

    bool isCountingDown;
    int countdownTime;






    void renderizar() {
        SDL_RenderClear(renderer);

        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        if (gameOver) {
            renderGameOver();
        } else {
            SDL_Rect foodRect = { food.x, food.y, CELL_SIZE, CELL_SIZE };
            SDL_RenderCopy(renderer, appleTexture, NULL, &foodRect);

            renderSnake();

            SDL_SetRenderDrawColor(renderer, currentLevel == LEVEL_1 ? 128 : (currentLevel == LEVEL_2 ? 64 : 32), 128, 128, 255);
            for (const auto& wall : walls) {
                SDL_Rect wallRect = { wall.x, wall.y, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &wallRect);
            }

            renderLevelTimer();

            SDL_Color white = {255, 255, 255, 255};
            scoreBoard.renderText(("Score: " + std::to_string(score)).c_str(), 9, 9, white);

            if (!isGameStarted) {
                renderStartMessage();
            }
            if (gameState == PAUSED && !isCountingDown) {
                renderPauseMenu();
            }
            if (isCountingDown) {
                renderCountdown();
            }
        }

        SDL_RenderPresent(renderer);
    }

    void renderLevelTimer() {
        int secondsLeft = levelTime / 1000;
        SDL_Color timerColor;
        if (secondsLeft > 60) {
            timerColor = {0, 255, 0, 255};  // Verde
        } else if (secondsLeft > 30) {
            timerColor = {255, 255, 0, 255};  // Amarillo
        } else {
            timerColor = {255, 0, 0, 255};  // Rojo
        }
        std::string timerText = "Time: " + std::to_string(secondsLeft);
        scoreBoard.renderText(timerText.c_str(), 9, 40, timerColor);
    }














        void startCountdown() {
        isCountingDown = true;
        countdownTime = 3000; // 3 segundos en milisegundos
        gameState = RUNNING; // Cambiamos el estado a RUNNING para que no se muestre el menú de pausa
    }


    void togglePause() {
        if (gameState == RUNNING) {
            gameState = PAUSED;
            Mix_PauseMusic();
        } else if (gameState == PAUSED) {
            startCountdown();
        }
    }

    void updateCountdown(long long deltaTime) {
        countdownTime -= deltaTime;
        if (countdownTime <= 0) {
            isCountingDown = false;
            gameState = RUNNING;
            Mix_ResumeMusic();
        }
    }
















    void renderCountdown() {
        SDL_Color white = {255, 255, 255, 255};
        std::string countdownText = "Reanudando en " + std::to_string((countdownTime / 1000) + 1);
        scoreBoard.renderText(countdownText.c_str(), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, white);
    }












































    void actualizar(long long deltaTime) {
        if (gameOver || gameState == PAUSED) return;

        if (isCountingDown) {
            updateCountdown();
            return;
        }

        Point newHead = snake.front();
        switch (direction) {
            case UP:    newHead.y -= CELL_SIZE; break;
            case DOWN:  newHead.y += CELL_SIZE; break;
            case LEFT:  newHead.x -= CELL_SIZE; break;
            case RIGHT: newHead.x += CELL_SIZE; break;
        }

        if (verificarColision(newHead)) {
            handleGameOver();
            return;
        }

        snake.insert(snake.begin(), newHead);

        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            generarComida();
            Mix_PlayChannel(-1, eatSound, 0);
        } else {
            snake.pop_back();
        }
    }




    void handlePauseInput(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) {
                selectedOption = (selectedOption - 1 + 4) % 4;  // Ahora son 4 opciones
            } else if (event.key.keysym.sym == SDLK_DOWN) {
                selectedOption = (selectedOption + 1) % 4;  // Ahora son 4 opciones
            } else if (event.key.keysym.sym == SDLK_RETURN) {
                executePauseOption();
            }
        }
    }

    void renderPauseMenu() {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color selectedColor = {0, 255, 0, 255};

        scoreBoard.renderText("PAUSED", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 150, white);
        scoreBoard.renderText("Reanudar", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 75, selectedOption == 0 ? selectedColor : white);
        scoreBoard.renderText("Reiniciar Juego", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, selectedOption == 1 ? selectedColor : white);
        scoreBoard.renderText("Menu Principal", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 25, selectedOption == 2 ? selectedColor : white);
        scoreBoard.renderText("Salir del Juego", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 75, selectedOption == 3 ? selectedColor : white);
    }

    void executePauseOption() {
        switch (selectedOption) {
            case 0: // Reanudar
                startCountdown();
                break;
            case 1: // Reiniciar Juego
                gameState = RUNNING;
                reiniciarMusica();
                resetGame();
                break;
            case 2: // Menu Principal
                returnToMainMenu();
                break;
            case 3: // Salir del Juego
                exit(0);
                break;
        }
    }


    void updateCountdown() {
        static Uint32 lastCountdownUpdate = SDL_GetTicks();
        Uint32 currentTime = SDL_GetTicks();

        if (currentTime - lastCountdownUpdate >= 1000) {
            countdownTime--;
            lastCountdownUpdate = currentTime;

            if (countdownTime <= 0) {
                isCountingDown = false;
                gameState = RUNNING;
                Mix_ResumeMusic();
                lastUpdateTime = std::chrono::steady_clock::now();
            }
        }
    }










        void handleGameOverInput(SDL_Keycode key) {
        if (key == SDLK_RETURN) {
            if (!playerName.empty()) {
                saveScore();
                returnToMainMenu();  // Llamamos a returnToMainMenu en lugar de resetGame
            }
        } else if (key == SDLK_ESCAPE) {
            returnToMainMenu();  // También volvemos al menú principal si se presiona ESC
        } else if (key == SDLK_BACKSPACE && !playerName.empty()) {
            playerName.pop_back();
        }
    }

    void saveScore() {
        if (playerName.empty()) {
            return;
        }

        nlohmann::json j;
        std::ifstream ifs("scores.json");

        if (ifs.good()) {
            ifs >> j;
        }
        ifs.close();

        bool playerExists = false;
        for (auto& entry : j["scores"]) {
            if (entry["name"] == playerName) {
                playerExists = true;
                if (score > entry["score"]) {
                    entry["score"] = score;
                    entry["difficulty"] = difficulty;
                    entry["time"] = gameTime / 1000; // Convertir a segundos
                }
                break;
            }
        }

        if (!playerExists) {
            nlohmann::json newScore;
            newScore["name"] = playerName;
            newScore["score"] = score;
            newScore["difficulty"] = difficulty;
            newScore["time"] = gameTime / 1000; // Convertir a segundos
            j["scores"].push_back(newScore);
        }

        std::ofstream ofs("scores.json");
        ofs << j.dump(4);
        ofs.close();

        SDL_StopTextInput();
    }

    void returnToMainMenu() {
        isRunning = false;
        Mix_HaltMusic();
        menu->show();
    }


    void generarParedesNivel3() {
        generarParedes();  // Generar las paredes exteriores

        // Definir el tamaño de los obstáculos
        int obstaculoSize = 3;

        // Crear obstáculos en las esquinas
        for (int i = 0; i < obstaculoSize; i++) {
            for (int j = 0; j < obstaculoSize; j++) {
                // Esquina superior izquierda
                walls.push_back({CELL_SIZE * (2 + i), CELL_SIZE * (2 + j)});
                // Esquina superior derecha
                walls.push_back({SCREEN_WIDTH - CELL_SIZE * (3 + i), CELL_SIZE * (2 + j)});
                // Esquina inferior izquierda
                walls.push_back({CELL_SIZE * (2 + i), SCREEN_HEIGHT - CELL_SIZE * (3 + j)});
                // Esquina inferior derecha
                walls.push_back({SCREEN_WIDTH - CELL_SIZE * (3 + i), SCREEN_HEIGHT - CELL_SIZE * (3 + j)});
            }
        }

        // Crear obstáculos en forma de "L" en el centro
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;
        for (int i = -2; i <= 2; i++) {
            walls.push_back({centerX + i * CELL_SIZE, centerY});
            walls.push_back({centerX, centerY + i * CELL_SIZE});
        }

        // Añadir obstáculos adicionales
        walls.push_back({centerX - 5 * CELL_SIZE, centerY});
        walls.push_back({centerX + 5 * CELL_SIZE, centerY});
        walls.push_back({centerX, centerY - 5 * CELL_SIZE});
        walls.push_back({centerX, centerY + 5 * CELL_SIZE});

        // Crear pequeños obstáculos aleatorios
        int numRandomObstacles = 5;
        for (int i = 0; i < numRandomObstacles; i++) {
            int x, y;
            do {
                x = (rand() % (SCREEN_WIDTH / CELL_SIZE - 4) + 2) * CELL_SIZE;
                y = (rand() % (SCREEN_HEIGHT / CELL_SIZE - 4) + 2) * CELL_SIZE;
            } while (verificarColision({x, y}));

            walls.push_back({x, y});
            walls.push_back({x + CELL_SIZE, y});
            walls.push_back({x, y + CELL_SIZE});
        }
    }

    bool cargarTexturas() {
        headTexture = loadTexture("assets/images/snake_head.png");
        bodyTexture = loadTexture("assets/images/snake_body.png");
        appleTexture = loadTexture("assets/images/apple.png");
        backgroundTexture = loadTexture("assets/images/background.png");
        return (headTexture != nullptr && bodyTexture != nullptr && appleTexture != nullptr && backgroundTexture != nullptr);
    }

    SDL_Texture* loadTexture(const char* path) {
        SDL_Surface* surface = IMG_Load(path);
        if (!surface) {
            std::cerr << "Error loading image: " << IMG_GetError() << std::endl;
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

    double calculateHeadRotation() {
        if (snake.size() < 2) {
            switch (direction) {
                case RIGHT: return 180.0;
                case LEFT:  return 0.0;
                case DOWN:  return 270.0;
                case UP:    return 90.0;
                default:    return 180.0;
            }
        }

        Point head = snake[0];
        Point neck = snake[1];

        if (head.x > neck.x) return 180.0;
        if (head.x < neck.x) return 0.0;
        if (head.y > neck.y) return 270.0;
        if (head.y < neck.y) return 90.0;

        return 180.0;
    }

    void renderSnake() {
        for (size_t i = 0; i < snake.size(); ++i) {
            SDL_Rect destRect = { snake[i].x, snake[i].y, CELL_SIZE, CELL_SIZE };
            if (i == 0) {
                double angle = calculateHeadRotation();
                SDL_RenderCopyEx(renderer, headTexture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
            } else {
                SDL_RenderCopy(renderer, bodyTexture, NULL, &destRect);
            }
        }
    }

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
        gameState = TRANSITION;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {255, 255, 255, 255};
        scoreBoard.renderText(mensaje.c_str(), SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, white);
        scoreBoard.renderText(subMensaje.c_str(), SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, white);
        scoreBoard.renderText("Presiona 'Y' para continuar o 'N' para regresar al menu.", SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 + 50, white);
        SDL_RenderPresent(renderer);

        bool continuar = esperarDecisionJugador();
        if (!continuar) {
            returnToMainMenu();
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
        isGameStarted = false;
        lastUpdateTime = std::chrono::steady_clock::now();
        gameState = RUNNING;
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
            SDL_Delay(10);  // Pequeña pausa para no saturar la CPU
        }
    }



    void handleGameOver() {
        gameOver = true;
        gameState = GAME_OVER;
        Mix_HaltMusic();
        SDL_StartTextInput();
    }

    void renderGameOver() {
        SDL_Color color = {255, 255, 255, 255};

        scoreBoard.renderText("Game Over", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, color);
        scoreBoard.renderText(("Score: " + std::to_string(score)).c_str(), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, color);
        scoreBoard.renderText("Enter your name:", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, color);

        if (!playerName.empty()) {
            scoreBoard.renderText(playerName.c_str(), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, color);
        } else {
            scoreBoard.renderText("No name entered", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, color);
        }

        scoreBoard.renderText("Press Enter to Save", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, color);
        scoreBoard.renderText("Press ESC to Restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 150, color);
    }



    void reiniciarMusica() {
        Mix_HaltMusic();
        Mix_PlayMusic(music, -1);
    }







    void resetGame() {
        levelTime = 90000;  // 90 segundos en milisegundos
        currentLevel = LEVEL_1;
        snake.clear();
        snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        direction = RIGHT;
        score = 0;
        currentLevel = LEVEL_1;
        walls.clear();
        generarParedes();
        generarComida();
        gameTime = 0;
        lastUpdateTime = std::chrono::steady_clock::now();
        gameOver = false;
        gameState = RUNNING;
        selectedOption = 0;
        gameOverOption = 0;
        isGameStarted = false;
        playerName.clear();
    }

    void renderStartMessage() {
        SDL_Color white = {255, 255, 255, 255};
        scoreBoard.renderText("Presiona una tecla para empezar", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 20, white);
    }
};

#endif // SNAKE_GAME_H
