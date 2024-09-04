#include <windows.h>
#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Engine engine;
    if (engine.init()) {
        engine.run();
    }
    engine.cleanup();
    return 0;
}



