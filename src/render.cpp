#include "render.h"

void concat_strings(char* result, const char* str1, const char* str2) {
    strcat(result, str1);
    strcat(result, str2);
}

void add_to_string(char* result, const char* str) {
    strcat(result, str);
}

void add_num(char* result, const int number) {
    char text[MAX_TEXT_CHARS + sizeof(char)];
    sprintf(text, "%d", number);
    strcat(result, text);
}

Render::Render(int width, int height, const char* path_to_textures) : window_width(width), window_height(height) {
    TTF_Init();

    assert(SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer) == 0);

    assert(loadTextures(path_to_textures));
    assert(loadFonts());

    border_width = ((float)window_width/(12*BORDER_SIZE*2)) * BORDER_SIZE;
    border_height = ((float)window_height/(22*BORDER_SIZE)) * BORDER_SIZE ;
    block_width = ((float)window_width/(12*BORDER_SIZE*2)) * BLOCK_SIZE;
    block_height = ((float)window_height/(22*BORDER_SIZE)) * BLOCK_SIZE;

}

Render::~Render() {
    TTF_CloseFont(font);
    TTF_CloseFont(font_big);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

Screen Render::blockToScreen(Block b) {
    return {b.col * block_width, b.row * block_height};
}

ScreenRect Render::blockToScreen(BlockRect b) {
    return {b.col * block_width, b.row * block_height, b.end_col * block_width, b.end_row * block_height};
}

void Render::renderLayers() {
    if (SDL_GetRenderTarget(renderer)) {
        SDL_SetRenderTarget(renderer, NULL);
    }
    
    SDL_RenderClear(renderer);
    for (int i = 0; i < num_of_layers; i++) {
        if (layers[i]->is_visible)
            SDL_RenderCopy(renderer, layers[i]->texture, NULL, &layers[i]->rect);
    }
    SDL_RenderPresent(renderer);
}

unsigned int Render::getRelativeWidth() {
    return block_width;
}

unsigned int Render::getRelativeHeight() {
    return block_height;
}

void Render::renderLayer(LayerId id) {
    if (id >= num_of_layers)
        return;
    
    if (SDL_GetRenderTarget(renderer)) {
        SDL_SetRenderTarget(renderer, NULL);
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, layers[id]->texture, NULL, &layers[id]->rect);
    SDL_RenderPresent(renderer);
}

LayerId Render::createLayer(ScreenRect rect) {
    if (num_of_layers >= MAX_LAYERS)
        return -1;

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    if (!texture)
        return -1;

    Layer* layer = new Layer();
    layer->texture = texture;
    layer->rect = SDL_Rect{rect.x, rect.y, rect.w, rect.h};
    layers[num_of_layers] = layer;
    num_of_layers++;

    return num_of_layers - 1;
}

bool Render::setLayerVisibility(LayerId id, bool is_visible) {
    if (id < 0 || id >= num_of_layers)
        return false;

    layers[id]->is_visible = is_visible;
    return true;
}

bool Render::clearLayer(LayerId id) {
    if (id < 0 || id >= num_of_layers)
        return false;
    
    if (SDL_GetRenderTarget(renderer) != layers[id]->texture) {
        if (SDL_SetRenderTarget(renderer, layers[id]->texture) < 0) {
            return false;
        }
    }

    SDL_RenderClear(renderer);

    return true;
}

bool Render::changeLayerPriority(LayerId id, LayerId new_id) {
    if (id < 0 || id >= num_of_layers || new_id < 0 || new_id >= num_of_layers || id == new_id)
        return false;

    if (id > new_id) {
        Layer* layer = layers[id];
        for (int i = id + 1; i <= new_id; i++) {
            layers[i - 1] = layers[i];
        }
        layers[new_id] = layer;
    }
    else {
        Layer* layer = layers[id];
        for (int i = id - 1; i >= new_id; i--) {
            layers[i + 1] = layers[i];
        }
        layers[new_id] = layer;
    }

    return true;
}

bool Render::putOnLayer(LayerId layer_id, RenderObjectType object_type, ObjectId id, Screen p, unsigned char transparency) {
    ScreenRect rect = {p.x, p.y, 0, 0};

    switch (object_type) {
    case RenderObjectType::Block:
        rect = {p.x, p.y, block_width, block_height};
        break;

    case RenderObjectType::Text:
        rect.x = p.x; rect.y = p.y;
        SDL_QueryTexture(texts[id], NULL, NULL, &rect.w, &rect.h);
        break;

    case RenderObjectType::Custom:
        rect.x = p.x; rect.y = p.y;
        SDL_QueryTexture(custom_textures[id], NULL, NULL, &rect.w, &rect.h);
        break;
    
    case RenderObjectType::Layer:
        rect.x = p.x; rect.y = p.y;
        SDL_QueryTexture(layers[id]->texture, NULL, NULL, &rect.w, &rect.h);
        break;
    }


    return putOnLayer(layer_id, object_type, id, rect, transparency);
}

bool Render::putOnLayer(LayerId layer_id, RenderObjectType object_type, ObjectId id, ScreenRect rect, unsigned char transparency) {
    if (layer_id < 0 || layer_id >= num_of_layers)
        return false;

    if (SDL_GetRenderTarget(renderer) != layers[layer_id]->texture) {
        if (SDL_SetRenderTarget(renderer, layers[layer_id]->texture) < 0) {
            return false;
        }
    }

    SDL_Rect sdl_rect;

    switch (object_type) {
    case RenderObjectType::Block:
        if (id < 1 || id >= MAX_COLORS)
            return false;
        
        sdl_rect = {rect.x, rect.y, rect.w, rect.h};
        if (transparency < 255)
            SDL_SetTextureAlphaMod(block_colors[id], transparency);

        SDL_RenderCopy(renderer, block_colors[id], NULL, &sdl_rect);
        if (transparency < 255)
            SDL_SetTextureAlphaMod(block_colors[id], 255);
        break;

    case RenderObjectType::Text:
        if (id < 0 || id >= num_of_texts)
            return false;

        sdl_rect = {rect.x, rect.y, rect.w, rect.h};
        if (transparency < 255) 
            SDL_SetTextureAlphaMod(texts[id], transparency);

        SDL_RenderCopy(renderer, texts[id], NULL, &sdl_rect);
        if (transparency < 255) 
            SDL_SetTextureAlphaMod(texts[id], 255);

        break;

    case RenderObjectType::Custom:
        sdl_rect = {rect.x, rect.y, rect.w, rect.h};
        if (transparency < 255)
            SDL_SetTextureAlphaMod(custom_textures[id], transparency);

        SDL_RenderCopy(renderer, custom_textures[id], NULL, &sdl_rect);
        if (transparency < 255)
            SDL_SetTextureAlphaMod(custom_textures[id], 255);
        break;

    case RenderObjectType::Layer:
        if (id < 0 || id > num_of_layers)
            return false;

        sdl_rect = {rect.x, rect.y, rect.w, rect.h};
        if (transparency < 255)
            SDL_SetTextureAlphaMod(layers[id]->texture, transparency);

        SDL_RenderCopy(renderer, layers[id]->texture, NULL, &sdl_rect);
        if (transparency < 255)
            SDL_SetTextureAlphaMod(layers[id]->texture, 255);

        break;
    }

    return true;
}

bool Render::deleteLayer(LayerId id) {
    if (id < 0 || id >= num_of_layers)
        return false;

    if (!layers[id])
        return false;

    SDL_DestroyTexture(layers[id]->texture);
    delete layers[id];

    for (int i = id + 1; i < num_of_layers; i++) {
        layers[i - 1] = layers[i];
    }

    num_of_layers--;
    layers[num_of_layers] = NULL;
    return true;
}

TextId Render::createText(FontType font_type, int number, Color color) {
    char text[MAX_TEXT_CHARS + sizeof(char)];
    sprintf(text, "%d", number);
    return createText(font_type, text, color);
}

TextId Render::createText(FontType font_type, const char* text, Color color) {
    if (strlen(text) > MAX_TEXT_CHARS)
        return -1;

    if (num_of_texts >= MAX_TEXTS)
        return -1;

    SDL_Texture* texture = NULL;
    SDL_Surface* surface = NULL;

    switch (font_type) {
    case FontType::Normal:
        surface = TTF_RenderUTF8_Solid(font, text, getColor(color));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture)
            return -1;
        break;

    case FontType::Big:
        surface = TTF_RenderUTF8_Solid(font_big, text, getColor(color));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture)
            return -1;
        break;
    }

    texts[num_of_texts] = texture;
    num_of_texts++;
    return (num_of_texts - 1);
}

bool Render::updateText(TextId id, FontType font_type, unsigned int number, Color color) {
    char text[MAX_TEXT_CHARS + sizeof(char)];
    sprintf(text, "%d", number);
    return updateText(id, font_type, text, color);
}

bool Render::updateText(TextId id, FontType font_type, const char* text, Color color) {
    if (id < 0 || id >= num_of_texts || !texts[id])
        return false;

    SDL_Texture* texture = NULL;
    SDL_Surface* surface = NULL;

    switch (font_type) {
    case FontType::Normal:
        surface = TTF_RenderUTF8_Solid(font, text, getColor(color)); 
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture)
            return false;
        break;

    case FontType::Big:
        surface = TTF_RenderUTF8_Solid(font_big, text, getColor(color)); 
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture)
            return false;
        break;
    }

    SDL_DestroyTexture(texts[id]);
    texts[id] = texture;

    return true;
}

bool Render::deleteText(TextId id) {
    if (id < 0 || id >= num_of_texts)
        return false;

    if (!texts[id])
        return false;

    SDL_DestroyTexture(texts[id]);
    for (int i = id + 1; i < num_of_texts; i++) {
        texts[i - 1] = texts[i];
    }

    num_of_texts--;
    texts[num_of_texts] = NULL;
    return true;
}

int Render::getCursorId() {
    return cursor_id;
}

bool tryLoadTextures(const char* pack_name) {
    return false;
}

SDL_Color Render::getColor(Color color) {
    SDL_Color sdl_color = {250, 250, 250};

    switch (color)
    {
    case Color::Grey:
        sdl_color = {200, 200, 200};
        break;

    case Color::White:
        sdl_color = {250, 250, 250};
        break;

    case Color::Red:
        sdl_color = {250, 50, 50};
        break;

    case Color::Green:
        sdl_color = {50, 250, 50};
        break;

    case Color::Blue:
        sdl_color = {50, 50, 250};
        break;

    default:
        sdl_color = {250, 250, 250};
        break;
    }

    return sdl_color;
}

bool Render::loadTextures(const char* path_to_textures) {
    block_colors[1] = IMG_LoadTexture(renderer, "./textures/default/cyan.png");
    block_colors[2] = IMG_LoadTexture(renderer, "./textures/default/yellow.png");
    block_colors[3] = IMG_LoadTexture(renderer, "./textures/default/violet.png");
    block_colors[4] = IMG_LoadTexture(renderer, "./textures/default/green.png");
    block_colors[5] = IMG_LoadTexture(renderer, "./textures/default/red.png");
    block_colors[6] = IMG_LoadTexture(renderer, "./textures/default/blue.png");
    block_colors[7] = IMG_LoadTexture(renderer, "./textures/default/orange.png");
    block_colors[8] = IMG_LoadTexture(renderer, "./textures/default/blocked.png");
    block_colors[9] = IMG_LoadTexture(renderer, "./textures/default/borders.png");

    custom_textures[0] = IMG_LoadTexture(renderer, "./textures/default/background.png");
    if (custom_textures[1] = IMG_LoadTexture(renderer, "./textures/default/cursor.png"))\
        cursor_id = 1;
    custom_textures[2] = IMG_LoadTexture(renderer, "./textures/default/grid.png");

    for (int i = 1; i < MAX_COLORS; i++) {
        SDL_SetTextureBlendMode(block_colors[i], SDL_BLENDMODE_BLEND);
    }

    return (block_colors[1] &&
        block_colors[2] &&
        block_colors[3] &&
        block_colors[4] &&
        block_colors[5] &&
        block_colors[6] &&
        block_colors[7] &&
        block_colors[8] &&
        block_colors[9] &&
        custom_textures[0] &&
        custom_textures[1] &&
        custom_textures[2]);
}

bool Render::loadFonts() {
    font = TTF_OpenFont("./fonts/VT323-Regular.ttf", 40);
    font_big = TTF_OpenFont("./fonts/VT323-Regular.ttf", 80);

    if (font == NULL || font_big == NULL)
        return false;

    return true;
}

