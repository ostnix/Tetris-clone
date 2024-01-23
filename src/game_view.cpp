#include "game_view.h"

GameView::GameView(int width, int height) {
    getPacksList();

    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        records_text[i] = -1;
    }

    for (int i = 0; i < MAX_TEXTS; i++) {
        texts[i] = -1;
    }

    render = new Render(width, height);

    game_view = render->createLayer({0, 0, width, height});
    createTexts();
    redrawBackground(); 

    game_grid = render->createLayer(render->blockToScreen(BlockRect{1, 1, 10, 20}));
    ingame_menu = render->createLayer(render->blockToScreen(BlockRect{5, 5, 15, 15}));
    render->setLayerVisibility(ingame_menu, false);
}

GameView::~GameView() {
    delete render;
}

void GameView::draw(State& state) {
    if (state.paused) {           
        render->putOnLayer(game_view, RenderObjectType::Layer, game_grid, render->blockToScreen(BlockRect{1, 1, 10, 20}));            
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Pause], render->blockToScreen(Block{8, 10}));    
        render->renderLayer(game_view);
        return;
    }

    if (state.game_ended) {
        render->putOnLayer(game_view, RenderObjectType::Layer, game_grid, render->blockToScreen(BlockRect{1, 1, 10, 20}));
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::GameEnd], render->blockToScreen(Block{8, 10}));
        render->renderLayer(game_view);
        return;
    }

    if (!state.rerender)
        return;

    render->renderLayers();
}

void GameView::updateGrid(State& state) {
    render->clearLayer(game_grid);
    render->putOnLayer(game_grid, RenderObjectType::Custom, 2, render->blockToScreen(BlockRect{0, 0, 10, 20}));

    for (int col = 0; col < GAME_FIELD_WIDTH; col++) {
        for (int row = 0; row < GAME_FIELD_HEIGHT; row++) {
            if (state.grid[col][row].cell_color != CellColor::None) {
                render->putOnLayer( game_grid, 
                                    RenderObjectType::Block, 
                                    (unsigned int)state.grid[col][row].cell_color, 
                                    render->blockToScreen(Block{col, row}), 
                                    state.grid[col][row].transparency);
            }
        }
    }

    putTetromino(state.tetrominoes[ACTIVE], game_grid);
    if (state.shadow_enabled) {
        putTetromino(state.tetrominoes[SHADOW], game_grid);
    }
}

void GameView::updateScreen(State& state) {
    if (state.enter_player_name) {
        drawNewHighScore(state);
        render->updateText(player_name, FontType::Normal, "", Color::Blue);
        return;
    }

    switch (state.context) {
    case MenuType::Game:
        drawGame(state);
        break;

    case MenuType::HighScore:
        drawHighScore(state);
        break;
    
    case MenuType::InGameMenu:
        drawIngameMenu(state);
        break;
    
    case MenuType::MainMenu:
        drawMainMenu(state);
        break;
    
    case MenuType::Settings:
        drawSettings(state);
        break;

    case MenuType::TexturePacks:
        drawTexturePacksList(state);
        break;
    }
}

bool GameView::tryLoadTexturePack(unsigned int pack_index) {
    assert(render->tryLoadTextures(pack_folders_names[pack_index]));
    return true;
}

unsigned int GameView::getNumberOfTexturePacks() {
    return number_of_texture_packs;
}

void GameView::redrawBackground() {
    render->clearLayer(game_view);
    render->putOnLayer(game_view, RenderObjectType::Custom, 0, {0, 0, 650, 650});

    // Game grid borders
    for (int row = 0; row < (GAME_FIELD_HEIGHT + 2); row++) {
        for (int col = 0; col < (GAME_FIELD_WIDTH + 2); col++) {
            if (col > 0 && col < (GAME_FIELD_WIDTH + 1)) {
                if (row == 0 || row == (GAME_FIELD_HEIGHT + 1)) {
                    render->putOnLayer(game_view, RenderObjectType::Block, (unsigned int)CellColor::Grey, render->blockToScreen(Block{col, row}));
                }
            }
            else {
                render->putOnLayer(game_view, RenderObjectType::Block, (unsigned int)CellColor::Grey, render->blockToScreen(Block{col, row}));
            }
        }
    }

    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Score], render->blockToScreen(Block{12, 1}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::ScoreVal], render->blockToScreen(Block{16, 1}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Level], render->blockToScreen(Block{12, 3}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::LevelVal], render->blockToScreen(Block{16, 3}));
}

void GameView::showPause() {
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Pause], render->blockToScreen(Block{8, 10}));
}

void GameView::createTexts() {
    texts[Texts::Next] = render->createText(FontType::Normal, "Next:", Color::Grey);
    texts[Texts::Score] = render->createText(FontType::Normal, "Score:", Color::Grey);
    texts[Texts::ScoreVal] = render->createText(FontType::Normal, 0, Color::Grey);
    texts[Texts::Level] = render->createText(FontType::Normal, "Level:", Color::Grey);
    texts[Texts::LevelVal] = render->createText(FontType::Normal, 0, Color::Grey);
    texts[Texts::Holded] = render->createText(FontType::Normal, "Holded:", Color::Grey);
    texts[Texts::Shadow] = render->createText(FontType::Normal, "Shadow:", Color::Grey);
    texts[Texts::Back] = render->createText(FontType::Normal, "Back", Color::Grey);
    texts[Texts::CantLoadTexture] = render->createText(FontType::Normal, "Can't Load Texture", Color::Red);
    texts[Texts::EnterName] = render->createText(FontType::Normal, "Enter name:", Color::Green);

    texts[Texts::On] = render->createText(FontType::Normal, "On", Color::Green);
    texts[Texts::Off] = render->createText(FontType::Normal, "Off", Color::Red);;
    
    texts[Texts::Exit] = render->createText(FontType::Big, "Exit", Color::Grey);
    texts[Texts::NewGame] = render->createText(FontType::Big, "New Game", Color::Grey);
    texts[Texts::Settings] = render->createText(FontType::Big, "Settings", Color::Grey);
    texts[Texts::Resume] = render->createText(FontType::Big, "Resume", Color::Grey);
    texts[Texts::MainMenu] = render->createText(FontType::Big, "Main Menu", Color::Grey);
    texts[Texts::HighScore] = render->createText(FontType::Big, "High Score", Color::Grey);
    texts[Texts::Pause] = render->createText(FontType::Big, "PAUSE", Color::Grey);
    texts[Texts::GameEnd] = render->createText(FontType::Big, "GAME END", Color::Red);
    texts[Texts::TexturePacks] = render->createText(FontType::Big, "Texture packs", Color::Grey);
    texts[Texts::NewHighScore] = render->createText(FontType::Big, "New High Score!!!", Color::Green);

    for (int i = 0; i < number_of_texture_packs; i++) {
        texture_pack_folders[i] = render->createText(FontType::Normal, pack_folders_names[i], Color::Grey);
    }
}

void GameView::putTetromino(Tetromino& tetromino, LayerId id) {
    if (tetromino.getColor() != CellColor::None) {
        for (int i = 0; i < tetromino.num_of_cells; i++) {
            Block block{tetromino.cells[i].col, tetromino.cells[i].row};
            render->putOnLayer(id,
                RenderObjectType::Block,
                (unsigned int)tetromino.cells[i].cell_color,
                render->blockToScreen(block),
                tetromino.cells[i].transparency);
        }
    }
}

void GameView::drawGame(State& state) {
    if (state.update_score) {
        render->updateText(texts[Texts::ScoreVal], FontType::Normal, state.score, Color::Grey);
        state.update_score = false;
    }
    
    if (state.update_level) {
        render->updateText(texts[Texts::LevelVal], FontType::Normal, state.level, Color::Grey);
        state.update_level = false;
    }

    redrawBackground();
    if (state.show_next_piece_enabled) {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Next], render->blockToScreen(Block{12, 5}));
        state.tetrominoes[NEXT].setColRow(18, 5);
        putTetromino(state.tetrominoes[NEXT], game_view);
    }

    if (state.hold_enabled) {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Holded], render->blockToScreen(Block{12, 9}));
        state.tetrominoes[HOLDED].setColRow(18, 9);
        putTetromino(state.tetrominoes[HOLDED], game_view);
    }
}

void GameView::drawIngameMenu(State& state) {
    render->clearLayer(ingame_menu);
    render->putOnLayer(ingame_menu, RenderObjectType::Custom, 3, render->blockToScreen(Block{0, 0}));
    render->putOnLayer(ingame_menu, RenderObjectType::Text, texts[Texts::Resume], render->blockToScreen(Block{3, 1}));
    render->putOnLayer(ingame_menu, RenderObjectType::Text, texts[Texts::MainMenu], render->blockToScreen(Block{3, 4}));
    render->putOnLayer(ingame_menu, RenderObjectType::Custom, 1, render->blockToScreen(Block{1, 1 + state.cursor_position * 4}));
    render->putOnLayer(game_view, RenderObjectType::Layer, game_grid, render->blockToScreen(BlockRect{1, 1, 10, 20}));
    render->putOnLayer(game_view, RenderObjectType::Layer, ingame_menu, render->blockToScreen(BlockRect{5, 5, 15, 15}));

    render->renderLayer(game_view);
}

void GameView::drawMainMenu(State& state) {
    render->clearLayer(game_view);
    render->putOnLayer(game_view, RenderObjectType::Custom, 3, render->blockToScreen(Block{0, 0}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::NewGame], render->blockToScreen(Block{7, 4}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Settings], render->blockToScreen(Block{7, 7}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::HighScore], render->blockToScreen(Block{7, 10}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::TexturePacks], render->blockToScreen(Block{7, 13}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Exit], render->blockToScreen(Block{7, 16}));
    render->putOnLayer(game_view, RenderObjectType::Custom, 1, render->blockToScreen(Block{5, 4 + state.cursor_position * 3}));
    render->renderLayer(game_view);

}

void GameView::drawSettings(State& state) {
    render->clearLayer(game_view);
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Settings], render->blockToScreen(Block{6, 1}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Shadow], render->blockToScreen(Block{7, 4}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Holded], render->blockToScreen(Block{7, 6}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Next], render->blockToScreen(Block{7, 8}));
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Back], render->blockToScreen(Block{7, 10}));
    
    if (state.shadow_enabled) {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::On], render->blockToScreen(Block{12, 4}));
    }
    else {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Off], render->blockToScreen(Block{12, 4}));
    }
        
    if (state.hold_enabled) {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::On], render->blockToScreen(Block{12, 6}));
    }
    else {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Off], render->blockToScreen(Block{12, 6}));
    }

    if (state.show_next_piece_enabled) {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::On], render->blockToScreen(Block{12, 8}));
    }
    else {
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Off], render->blockToScreen(Block{12, 8}));
    }

    render->putOnLayer(game_view, RenderObjectType::Custom, 1, render->blockToScreen(Block{5, 4 + state.cursor_position * 2}));
    render->renderLayer(game_view);
}

void GameView::drawHighScore(State& state) {
    render->clearLayer(game_view);
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::HighScore], render->blockToScreen(Block{7, 2}));
    
    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        if (records_text[i] == -1) {
            char record[MAX_TEXT_CHARS] = {'\0'};
            concat_strings(record, state.records[i].name, ": ");
            add_num(record, state.records[i].score);
            records_text[i] = render->createText(FontType::Normal, record, Color::Blue);
        }
        else if (state.new_high_score) {
            state.new_high_score = false;
            char record[MAX_TEXT_CHARS] = {'\0'};
            concat_strings(record, state.records[i].name, ": ");
            add_num(record, state.records[i].score);
            records_text[i] = render->updateText(records_text[i], FontType::Normal, record, Color::Blue);
        }

        render->putOnLayer(game_view, RenderObjectType::Text, records_text[i], render->blockToScreen(Block{7, 5 + i * 2}));
    }

    render->renderLayer(game_view);
}

void GameView::drawTexturePacksList(State& state) {
    render->clearLayer(game_view);

    for (int i = 0; i < number_of_texture_packs; i++) {
        render->putOnLayer(game_view, RenderObjectType::Text, texture_pack_folders[i], render->blockToScreen(Block{5, 2 + i * 2}));
    }
    render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Back], render->blockToScreen(Block{5, 2 + (int)number_of_texture_packs * 2}));
    render->putOnLayer(game_view, RenderObjectType::Custom, 1, render->blockToScreen(Block{1, 2 + state.cursor_position * 2}));
    render->renderLayer(game_view);
}

void GameView::drawNewHighScore(State& state) {
    render->putOnLayer(game_view, RenderObjectType::Layer, game_grid, render->blockToScreen(BlockRect{1, 1, 10, 20}));
    render->clearLayer(ingame_menu);
    if (player_name == -1) {
        player_name = render->createText(FontType::Normal, state.player_name, Color::Blue);
    }
    else {
        render->updateText(player_name, FontType::Normal, state.player_name, Color::Blue);
    }
    render->putOnLayer(ingame_menu, RenderObjectType::Text, texts[Texts::NewHighScore], render->blockToScreen(Block{0, 0}));
    render->putOnLayer(ingame_menu, RenderObjectType::Text, texts[Texts::EnterName], render->blockToScreen(Block{1, 5}));
    render->putOnLayer(ingame_menu, RenderObjectType::Text, player_name, render->blockToScreen(Block{1, 7}));
    render->putOnLayer(game_view, RenderObjectType::Layer, ingame_menu, render->blockToScreen(BlockRect{5, 5, 15, 15}));
    render->renderLayer(game_view);

    state.new_high_score = false;
}

void GameView::getPacksList() {
    const char* path = "./textures";

    DIR* dir = opendir(path);

    struct dirent* entry = readdir(dir);
    entry = readdir(dir);
    entry = readdir(dir);

    while (entry != NULL) {
        if (entry->d_type == DT_DIR && number_of_texture_packs < MAX_TEXTURE_PACKS) {
            strcpy(pack_folders_names[number_of_texture_packs], entry->d_name);
            number_of_texture_packs++;
        }

        entry = readdir(dir);
    }

    closedir(dir);
}