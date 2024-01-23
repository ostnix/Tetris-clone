#include "tetris.h"

int main(int argc, char* argv[]) {
    int width = 650;
    int height = 650;

    Tetris* tetris = new Tetris(width, height);

    tetris->start();

    return 0;
}
