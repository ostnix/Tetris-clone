#include "tetris.h"

Tetris::Tetris(int width, int height) {
    game_view = new GameView(width, height);
    input = new Input();
    sound = new Sound();
    game_logic = new GameLogic(state);
    high_score = new HighScore();

    state.records = high_score->getRecords();
    state.context = MenuType::MainMenu;

    assert(sound->loadSounds());
}

Tetris::~Tetris() {
    delete game_logic;
    delete sound;
    delete input;
    delete game_view;
    delete high_score;
}

void Tetris::start() {
    gameLoop();
}

void Tetris::gameLoop() {
    game_view->updateScreen(state);

    while(!state.quit_game) {
        handlePlayerAction();
        gameStep();
        sound->playSound(state.sound_to_play);
        state.sound_to_play = SoundEffect::None;
        timer.wait();
    }
}

void Tetris::handlePlayerAction() {
    state.last_action = PlayerAction::None;

    while ((state.last_action = input->getEvent()) != PlayerAction::None) {

        if (state.last_action == PlayerAction::Exit) {
            state.quit_game = true;
            return;
        }

        if (state.last_action == PlayerAction::SystemKey)
            return;

        switch (state.context) {
        case MenuType::Game:
            game();
            break;

        case MenuType::HighScore:
            playMenuSound();
            highScore();
            break;

        case MenuType::InGameMenu:
            playMenuSound();
            ingameMenu();
            break;

        case MenuType::MainMenu:
            playMenuSound();
            mainMenu();
            break;


        case MenuType::Settings:
            playMenuSound();
            settings();
            break;
        
        case MenuType::TexturePacks:
            playMenuSound();
            texturePacks();
            break;
        } 
    }
}

void Tetris::playMenuSound() {
    if (state.last_action == PlayerAction::Down || state.last_action == PlayerAction::Up) {
        state.sound_to_play = SoundEffect::MenuMove;
    }
    else if (state.last_action == PlayerAction::Enter) {
        state.sound_to_play = SoundEffect::MenuSelect;
    }
}

void Tetris::gameStep() {
    if (state.paused || state.new_high_score || state.game_ended)
        return;

    if (state.context == MenuType::Game) {
        if (state.update_level || state.update_score) {
            game_view->updateScreen(state);
        }

        if (state.delay == 0 || state.soft_drop || state.instant_drop) {
            if (state.instant_drop) {
                state.instant_drop = false;

                if (!game_logic->newTetromino()) {
                    state.game_ended = true;
                }
                else {
                    game_view->updateScreen(state);
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
                    game_view->updateScreen(state);
                }

            }

            game_view->updateGrid(state);
            state.delay = move_delay_per_level[state.level];
        }

        if (state.game_ended) {
            if (high_score->checkForNewRecord(state.score)) {
                state.sound_to_play = SoundEffect::NewHighScore;
                state.new_high_score = true;
                state.enter_player_name = true;
                input->keyboardTextInput(true);
                game_view->updateScreen(state);

            }
            else {
                state.sound_to_play = SoundEffect::GameEnd;
                game_view->draw(state);
            }
            return;
        }

        game_view->draw(state);
        state.delay--;
    }
}

void Tetris::game() {
    if (state.enter_player_name) {
        if (state.last_action == PlayerAction::SystemKey)
            return;

        char text_input[MAX_TEXT_CHARS] = {'\0'};
        input->getLastTextInput(text_input);
        if (state.last_action == PlayerAction::Enter) {
            high_score->writeNewRecord(state.player_name, state.score);
            input->keyboardTextInput(false);
            state.enter_player_name = false;
            state.context = MenuType::HighScore;
            game_view->updateScreen(state);
            return;
        }
        else if (state.last_action == PlayerAction::BackSpace) {
            state.sound_to_play = SoundEffect::Typing;
            int index = strlen(state.player_name);
            if (index > 0)
                state.player_name[index - 1] = '\0';

            game_view->updateScreen(state);
        }
        else if (text_input[0] != '\x1' && (strlen(state.player_name) + strlen(text_input) < MAX_TEXT_CHARS)) {
            state.sound_to_play = SoundEffect::Typing;
            strcat(state.player_name, text_input);
            game_view->updateScreen(state);
        }
        return;
    }

    if (state.paused) {
        state.paused = false;
        game_view->updateScreen(state);
        game_view->draw(state);
        return;
    }

    if (state.game_ended) {
        state.context = MenuType::MainMenu;
        game_view->updateScreen(state);
        return;
    }

    switch (state.last_action) {
    case PlayerAction::Left:
        if (game_logic->moveLeft())
            game_view->updateGrid(state);
        break;

    case PlayerAction::Right:
        if (game_logic->moveRight())
            game_view->updateGrid(state);
        break;

    case PlayerAction::Up:
        if (game_logic->rotate())
            game_view->updateGrid(state);
        break;

    case PlayerAction::Down:
        state.soft_drop = true;
        break;

    case PlayerAction::FastDrop:
        state.instant_drop = true;
        state.sound_to_play = SoundEffect::FastDrop;
        game_logic->moveDown(state.instant_drop);
        game_view->updateGrid(state);
        break;

    case PlayerAction::HoldPiece:
        if (game_logic->holdTetromino()) {
            game_view->updateScreen(state);
            game_view->updateGrid(state);
            game_view->draw(state);
        }
        break;

    case PlayerAction::Pause:
        state.paused = true;
        game_view->draw(state);
        break;

    case PlayerAction::ESC:
        state.context = MenuType::InGameMenu;
        game_view->updateScreen(state);
        break;

    case PlayerAction::OtherKey:
        break;
    }
}

void Tetris::highScore() {
    state.context = MenuType::MainMenu;
    game_view->updateScreen(state);
}

void Tetris::ingameMenu() {
    switch (state.last_action) {
    case PlayerAction::Up:
        state.cursor_position = (state.cursor_position + 1) % 2;
        game_view->updateScreen(state);
        break;

    case PlayerAction::Down:
        state.cursor_position = (state.cursor_position + 1) % 2;
        game_view->updateScreen(state);
        break;

    case PlayerAction::Enter:
        switch (state.cursor_position) {
        case 0:
            state.context = MenuType::Game;
            game_view->updateScreen(state);
            break;

        case 1:
            state.context = MenuType::MainMenu;
            game_view->updateScreen(state);
            break;
        }
        break;

    case PlayerAction::ESC:
        state.context = MenuType::Game;
        game_view->updateScreen(state);
        break;
    }
}

void Tetris::mainMenu() {
    switch (state.last_action) {
    case PlayerAction::Up:
        state.cursor_position = (state.cursor_position + 4) % 5;
        game_view->updateScreen(state);
        break;

    case PlayerAction::Down:
        state.cursor_position = (state.cursor_position + 1) % 5;
        game_view->updateScreen(state);
        break;

    case PlayerAction::Enter:
        switch (state.cursor_position) {
        case 0:
            state.player_name[0] = '\0';
            state.level = 0;
            state.score = 0;
            state.update_score = true;
            state.update_level = true;
            state.delay = move_delay_per_level[state.level];
            state.game_ended = false;
            state.context = MenuType::Game;
            game_logic->start();
            game_view->updateScreen(state);
            game_view->updateGrid(state);
            game_view->draw(state);
            break;

        case 1:
            state.cursor_position = 0;
            state.context = MenuType::Settings;
            game_view->updateScreen(state);
            break;

        case 2:
            state.cursor_position = 0;
            state.context = MenuType::HighScore;
            game_view->updateScreen(state);
            break;

        case 3:
            state.cursor_position = 0;
            state.context = MenuType::TexturePacks;
            game_view->updateScreen(state);
            break;

        case 4:
            state.quit_game = true;
            break;
        }
        break;
    }
}

void Tetris::settings() {
    switch (state.last_action) {
    case PlayerAction::Up:
        state.cursor_position = (state.cursor_position + 3) % 4;
        game_view->updateScreen(state);
        break;

    case PlayerAction::Down:
        state.cursor_position = (state.cursor_position + 1) % 4;
        game_view->updateScreen(state);
        break;

    case PlayerAction::Left:
    case PlayerAction::Right:
    case PlayerAction::Enter:
        switch (state.cursor_position) {
        case 0:
            state.shadow_enabled = !state.shadow_enabled;
            game_view->updateScreen(state);
            break;

        case 1:
            state.hold_enabled = !state.hold_enabled;
            game_view->updateScreen(state);
            break;

        case 2:
            state.show_next_piece_enabled = !state.show_next_piece_enabled;
            game_view->updateScreen(state);
            break;

        case 3:
            if (state.last_action == PlayerAction::Enter) {
                state.cursor_position = 0;
                state.context = MenuType::MainMenu;
                game_view->updateScreen(state);
            }
            break;
        }
        break;
    }
}

void Tetris::texturePacks() {
    unsigned int number_of_packs = game_view->getNumberOfTexturePacks();

    switch (state.last_action) {
    case PlayerAction::Up:
        state.cursor_position = (state.cursor_position + (number_of_packs)) % (number_of_packs + 1);
        game_view->updateScreen(state);
        break;

    case PlayerAction::Down:
        state.cursor_position = (state.cursor_position + 1) % (number_of_packs + 1);
        game_view->updateScreen(state);
        break;

    case PlayerAction::Enter:
        if (state.cursor_position == number_of_packs) {
            state.cursor_position = 0;
            state.context = MenuType::MainMenu;
            game_view->updateScreen(state);
        }
        else {
            if (game_view->tryLoadTexturePack(state.cursor_position)) {
                state.cursor_position = 0;
                state.context = MenuType::MainMenu;
                game_view->updateScreen(state);
            }
        }
        break;
    }
}