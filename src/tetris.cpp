#include "tetris.h"

Tetris::Tetris(int width, int height, unsigned int start_level) {
    game = new GameView(width, height);
    game_logic = new GameLogic(state);
    high_score = new HighScore();
    state.records = high_score->getRecords();

    state.context = MenuType::MainMenu;

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
    delete high_score;
}

void Tetris::start() {
    gameLoop();
}

void Tetris::gameLoop() {
    game->updateScreen(state);

    while(!state.quit_game) {
        handlePlayerAction();
        gameStep();
        timer.wait();
    }
}

void Tetris::handlePlayerAction() {
    PlayerAction last_action = PlayerAction::None;

    char text_input[MAX_TEXT_CHARS] = {'\0'};

    while ((last_action = input.getEvent(text_input)) != PlayerAction::None) {

        if (last_action == PlayerAction::Exit) {
            state.quit_game = true;
            return;
        }

        switch (state.context) {
        case MenuType::Game:
            if (state.enter_player_name) {
                if (last_action == PlayerAction::Enter) {
                    high_score->writeNewRecord(state.player_name, state.score);
                    input.keyboardTextInput(false);
                    state.enter_player_name = false;
                    state.context = MenuType::HighScore;
                    game->updateScreen(state);
                    return;
                }
                else if (last_action == PlayerAction::BackSpace) {
                    int index = strlen(state.player_name);
                    state.player_name[index - 1] = '\0';
                    game->updateScreen(state);
                }
                else if (text_input[0] != '\x1' && (strlen(state.player_name) + strlen(text_input) < MAX_TEXT_CHARS)) {
                    strcat(state.player_name, text_input);
                    game->updateScreen(state);
                }
                return;
            }

            if (state.paused) {
                state.paused = false;
                game->updateScreen(state);
                game->draw(state);
            }

            if (state.game_ended) {
                state.context = MenuType::MainMenu;
                game->updateScreen(state);
                return;
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
                state.context = MenuType::InGameMenu;
                game->updateScreen(state);
                break;

            case PlayerAction::OtherKey:
                break;
            }

            break;

        case MenuType::HighScore:
            state.context = MenuType::MainMenu;
            game->updateScreen(state);
            break;

        case MenuType::InGameMenu:
            switch (last_action) {
            case PlayerAction::Up:
                state.cursor_position = (state.cursor_position + 1) % 2;
                game->updateScreen(state);
                break;

            case PlayerAction::Down:
                state.cursor_position = (state.cursor_position + 1) % 2;
                game->updateScreen(state);
                break;

            case PlayerAction::Enter:
                switch (state.cursor_position) {
                case 0:
                    state.context = MenuType::Game;
                    game->updateScreen(state);
                    break;

                case 1:
                    state.context = MenuType::MainMenu;
                    game->updateScreen(state);
                    break;
                }
                break;
            }
            break;

        case MenuType::MainMenu:
            switch (last_action) {
            case PlayerAction::Up:
                state.cursor_position = (state.cursor_position + 4) % 5;
                game->updateScreen(state);
                break;

            case PlayerAction::Down:
                state.cursor_position = (state.cursor_position + 1) % 5;
                game->updateScreen(state);
                break;

            case PlayerAction::Enter:
                switch (state.cursor_position) {
                case 0:
                    state.player_name[0] = '\0';
                    state.level = 0;
                    state.score = 0;
                    state.delay = move_delay_per_level[state.level];
                    state.game_ended = false;
                    state.context = MenuType::Game;
                    game_logic->start();
                    game->updateScreen(state);
                    game->updateGrid(state);
                    game->draw(state);
                    break;

                case 1:
                    state.cursor_position = 0;
                    state.context = MenuType::Settings;
                    game->updateScreen(state);
                    break;

                case 2:
                    state.cursor_position = 0;
                    state.context = MenuType::HighScore;
                    game->updateScreen(state);
                    break;

                case 3:
                    state.cursor_position = 0;
                    state.context = MenuType::TexturePacks;
                    game->updateScreen(state);
                    break;

                case 4:
                    state.quit_game = true;
                    break;
                }
                break;
            }
            break;


        case MenuType::Settings:
            switch (last_action) {
            case PlayerAction::Up:
                state.cursor_position = (state.cursor_position + 3) % 4;
                game->updateScreen(state);
                break;

            case PlayerAction::Down:
                state.cursor_position = (state.cursor_position + 1) % 4;
                game->updateScreen(state);
                break;

            case PlayerAction::Enter:
                switch (state.cursor_position) {
                case 0:
                    state.shadow_enabled = !state.shadow_enabled;
                    game->updateScreen(state);
                    break;

                case 1:
                    state.hold_enabled = !state.hold_enabled;
                    game->updateScreen(state);
                    break;

                case 2:
                    state.show_next_piece_enabled = !state.show_next_piece_enabled;
                    game->updateScreen(state);
                    break;

                case 3:
                    state.cursor_position = 0;
                    state.context = MenuType::MainMenu;
                    game->updateScreen(state);
                    break;
                }
                break;
            }
            break;
        
        case MenuType::TexturePacks:
            unsigned int number_of_packs = game->getNumberOfTexturePacks();

            switch (last_action) {
            case PlayerAction::Up:
                state.cursor_position = (state.cursor_position + (number_of_packs)) % (number_of_packs + 1);
                game->updateScreen(state);
                break;

            case PlayerAction::Down:
                state.cursor_position = (state.cursor_position + 1) % (number_of_packs + 1);
                game->updateScreen(state);
                break;

            case PlayerAction::Enter:
                if (state.cursor_position == number_of_packs) {
                    state.cursor_position = 0;
                    state.context = MenuType::MainMenu;
                    game->updateScreen(state);
                }
                else {
                    if (game->tryLoadTexturePack(state.cursor_position)) {
                        state.cursor_position = 0;
                        state.context = MenuType::MainMenu;
                        game->updateScreen(state);
                    }
                }
                break;
            }

            break;
        } 
    }
}

void Tetris::gameStep() {
    if (state.paused || state.new_high_score || state.game_ended)
        return;

    if (state.context == MenuType::Game) {
        if (state.update_level || state.update_score) {
            game->updateScreen(state);
        }

        if (state.delay == 0 || state.soft_drop || state.instant_drop) {
            if (state.instant_drop) {
                state.instant_drop = false;

                if (!game_logic->newTetromino()) {
                    state.game_ended = true;
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
                    state.game_ended = true;
                }
                else {
                    game->updateScreen(state);
                }

            }

            game->updateGrid(state);
            state.delay = move_delay_per_level[state.level];
        }

        if (state.game_ended) {
            if (high_score->checkForNewRecord(state.score)) {
                state.new_high_score = true;
                state.enter_player_name = true;
                input.keyboardTextInput(true);
                game->updateScreen(state);

            }
            else {
                game->draw(state);
            }
            return;
        }

        game->draw(state);
        state.delay--;
    }
}