#include <windows.h>
#include "Engine.h"

int main(int argc, char* argv[]) {
    // Minimizar la ventana de la consola
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow != NULL) {
        ShowWindow(consoleWindow, SW_MINIMIZE);
    }


    Engine engine;
    if (engine.init()) {
        engine.run();
    }
    engine.cleanup();
    return 0;
}
