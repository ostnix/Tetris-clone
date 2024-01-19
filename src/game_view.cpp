#include "game.h"

GameView::GameView(int width, int height) {
    render = new Render(width, height);

    main_menu = render->createLayer({0, 0, width, height});
    game_view = render->createLayer({0, 0, width, height});
    createTexts();
    updateBackground(); 

    game_grid = render->createLayer(render->blockToScreen(BlockRect{1, 1, 10, 20}));
    ingame_menu = render->createLayer(render->blockToScreen(BlockRect{5, 5, 15, 15}));
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
    putTetromino(state.tetrominoes[SHADOW], game_grid);
}

void GameView::updateScreen(State& state) {
    switch (state.context) {
    case MenuType::Game:
        if (state.update_score) {
            render->updateText(texts[Texts::ScoreVal], FontType::Normal, state.score, Color::Grey);
            state.update_score = false;
        }
        
        if (state.update_level) {
            render->updateText(texts[Texts::LevelVal], FontType::Normal, state.level, Color::Grey);
            state.update_level = false;
        }

        updateBackground();
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Next], render->blockToScreen(Block{12, 5}));
        state.tetrominoes[NEXT].setColRow(18, 5);
        putTetromino(state.tetrominoes[NEXT], game_view);

        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Holded], render->blockToScreen(Block{12, 9}));
        state.tetrominoes[HOLDED].setColRow(18, 9);
        putTetromino(state.tetrominoes[HOLDED], game_view);

        break;

    case MenuType::HighScore:
        break;
    
    case MenuType::InGameMenu:
        break;
    
    case MenuType::MainMenu:
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::NewGame], render->blockToScreen(Block{7, 4}));
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Settings], render->blockToScreen(Block{7, 7}));
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::HighScore], render->blockToScreen(Block{7, 10}));
        render->putOnLayer(game_view, RenderObjectType::Text, texts[Texts::Exit], render->blockToScreen(Block{7, 13}));
        break;
    
    case MenuType::Settings:
        break;
    }
}

void GameView::updateBackground() {
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
    texts[Texts::Holded] = render->createText(FontType::Normal, "Holded:", Color::Grey);
    texts[Texts::Next] = render->createText(FontType::Normal, "Next:", Color::Grey);
    texts[Texts::Score] = render->createText(FontType::Normal, "Score:", Color::Grey);
    texts[Texts::ScoreVal] = render->createText(FontType::Normal, 0, Color::Grey);
    texts[Texts::Level] = render->createText(FontType::Normal, "Level:", Color::Grey);
    texts[Texts::LevelVal] = render->createText(FontType::Normal, 0, Color::Grey);
    texts[Texts::Holded] = render->createText(FontType::Normal, "Holded:", Color::Grey);
    texts[Texts::Shadow] = render->createText(FontType::Normal, "Shadow:", Color::Grey);

    texts[Texts::On] = render->createText(FontType::Normal, "On", Color::Green);
    texts[Texts::Off] = render->createText(FontType::Normal, "Off", Color::Red);;
    
    texts[Texts::Exit] = render->createText(FontType::Big, "Exit", Color::Grey);
    texts[Texts::NewGame] = render->createText(FontType::Big, "New Game", Color::Grey);
    texts[Texts::Settings] = render->createText(FontType::Big, "Settings", Color::Grey);
    texts[Texts::Resume] = render->createText(FontType::Big, "Resume", Color::Grey);
    texts[Texts::MainMenu] = render->createText(FontType::Big, "Main Menu", Color::Grey);
    texts[Texts::HighScore] = render->createText(FontType::Big, "High Score", Color::Grey);
    texts[Texts::Pause] = render->createText(FontType::Big, "PAUSE", Color::Grey);
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