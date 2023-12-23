#include "game.h"

int main(int argc, char* argv[]) {
    int width = 650;
    int height = 650;

    Game game(width, height);
    game.start();

    return 0;
}
