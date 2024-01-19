#include "tetris.h"

Tetris::Tetris(int width, int height, unsigned int start_level) {
    game = new Game(width, height);
    game_logic = new GameLogic(state);
    state.context = MenuType::Game;

    if (start_level <= 29) {
        state.level = start_level;
    }
    else {
        state.level = 29;
    }
}

Tetris::~Tetris() {
    delete game_logic;
    delete game;
}

void Tetris::start() {
    gameLoop();
}

void Tetris::gameLoop() {
    unsigned int delay = move_delay_per_level[state.level];
    game_logic->start();
    game->updateScreen(state);

    while(!state.quit_game) {
        handlePlayerAction();
        workoutGameContext();
        timer.wait();
    }
}

void Tetris::handlePlayerAction() {
    PlayerAction last_action = PlayerAction::None;
    while ((last_action = input.getEvent()) != PlayerAction::None) {

        if (last_action == PlayerAction::Exit) {
            state.quit_game = true;
            return;
        }

        switch (state.context) {
        case MenuType::Game:
            if (state.paused) {
                state.paused = false;
                game->updateScreen(state);
                game->draw(state);
            }

            switch (last_action) {
            case PlayerAction::Left:
                if (game_logic->moveLeft())
                    game->updateGrid(state);
                break;

            case PlayerAction::Right:
                if (game_logic->moveRight())
                    game->updateGrid(state);
                break;

            case PlayerAction::Up:
                if (game_logic->rotate())
                    game->updateGrid(state);
                break;

            case PlayerAction::Down:
                state.soft_drop = true;
                break;

            case PlayerAction::FastDrop:
                state.instant_drop = true;
                game_logic->moveDown(state.instant_drop);
                game->updateGrid(state);
                break;

            case PlayerAction::HoldPiece:
                if (game_logic->holdTetromino()) {
                    game->updateScreen(state);
                    game->updateGrid(state);
                    game->draw(state);
                }
                break;

            case PlayerAction::Pause:
                state.paused = true;
                game->draw(state);
                break;

            case PlayerAction::ESC:
                break;

            case PlayerAction::OtherKey:
                break;
            }

            break;

        case MenuType::HighScore:
            break;

        case MenuType::InGameMenu:
            switch (last_action) {
            case PlayerAction::Up:
                state.cursor_position = (state.cursor_position - 1) % 3;
                break;

            case PlayerAction::Down:
                state.cursor_position = (state.cursor_position + 1) % 3;
                break;

            case PlayerAction::Enter:
                switch (state.cursor_position) {
                case 1:
                    break;

                case 2:
                    break;

                case 3:
                    break;
                }
                break;
            }
            break;

        case MenuType::MainMenu:
            break;

        case MenuType::Settings:
            break;
        } 
    }
}

void Tetris::workoutGameContext() {
    if (state.paused)
        return;

    if (state.context == MenuType::Game) {
        if (state.update_level || state.update_score) {
            game->updateScreen(state);
        }

        if (delay == 0 || state.soft_drop || state.instant_drop) {
            if (state.instant_drop) {
                state.instant_drop = false;

                if (!game_logic->newTetromino()) {
                    state.quit_game = true;
                }
                else {
                    game->updateScreen(state);
                }
            }
            else if (!game_logic->moveDown()) {
                if (state.soft_drop && !state.soft_drop_instant_blocking) {
                    state.soft_drop = false;
                }
                else if (!game_logic->newTetromino()) {
                    state.quit_game = true;
                }
                else {
                    game->updateScreen(state);
                }

            }

            game->updateGrid(state);
            delay = move_delay_per_level[state.level];
        }
        game->draw(state);
        delay--;      
    }
    else {
        game->updateScreen(state);
    }
}