#ifndef __RENDER_H__
#define __RENDER_H__

#include <cassert>
#include "utils.h"

#ifdef _WIN32
    #include "SDL.h"
    #include "SDL_image.h"
    #include "SDL_ttf.h"
#elif __linux__
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
#endif

#include "tetromino.h"

//typedef int LabelId;
typedef int BlockId;
typedef int TextId;
typedef int BoxId;
typedef int LayerId;
typedef int ObjectId;

enum class FontType {Normal, Big};
enum class RenderObjectType {None, Block, Text, Box, Layer, Custom};
enum class Color {None, Grey, White, Red, Green, Blue, Black, Transparent};

struct Block {
    int col, row;
};

struct Screen { // for special cases(smooth movement for example)
    int x, y;
}; 

struct BlockRect {
    int col, row;
    int end_col, end_row;
};

struct ScreenRect {
    int x, y;
    int w, h;
};

static const unsigned int HIGH_SCORE_PLAYERS_NUMBER = 5;
//static const unsigned int MAXIMUM_NUM_OF_LABELS = 15;
static const unsigned int MAX_TEXT_CHARS = 30;
static const unsigned int BORDER_SIZE = 60;
static const unsigned int BLOCK_SIZE = 60;
static const unsigned int MAX_COLORS = 10;
static const unsigned int MAX_LAYERS = 10;
static const unsigned int MAX_TEXTS = 20;
//static const unsigned int BLOCKED_COLOR = 8;

void concat_strings(char* result, const char* str1, const char* str2);

struct Layer {
    SDL_Texture* texture;
    SDL_Rect rect;
};

class Render {
public:
    Render(int width, int height, const char* path_to_textures = "./textures/default");
    ~Render();

    Screen blockToScreen(Block coord);
    ScreenRect blockToScreen(BlockRect b);
    
    void renderLayers();
    void renderLayer(LayerId id);

    LayerId createLayer(ScreenRect rect);
    bool clearLayer(LayerId id);
    bool changeLayerPriority(LayerId id, LayerId new_id);
    bool putOnLayer(LayerId layer_id, RenderObjectType object_type, ObjectId id, Screen p, unsigned char transparency = 255);
    bool putOnLayer(LayerId layer_id, RenderObjectType object_type, ObjectId id, ScreenRect rect, unsigned char transparency = 255);
    bool deleteLayer(LayerId id);

    TextId createText(FontType font_type, int number, Color color);
    TextId createText(FontType font_type, const char* text, Color color);
    bool updateText(TextId id, FontType font_type, unsigned int number, Color color);
    bool updateText(TextId id, FontType font_type, const char* text, Color color);
    bool deleteText(TextId id);

    /*
    BoxId createBox(Color background_color, ScreenRect size);
    BoxId createBox(ObjectId id, RenderObjectType background_object_type, ScreenRect size);
    bool deleteBox(BoxId id);
    */

private:
    SDL_Color getColor(Color color);

    bool loadTextures(const char* path_to_textures);
    bool loadFonts();

    int window_width;
    int window_height;
    int block_width;
    int block_height;
    int border_width;
    int border_height;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Texture* block_colors[MAX_COLORS] = {NULL};
    SDL_Texture* custom_textures[3] = {NULL};
    SDL_Texture* texts[MAX_TEXTS] = {NULL};
    
    unsigned int num_of_texts = 0;
    
    Layer* layers[MAX_LAYERS] = {NULL};
    unsigned int num_of_layers = 0;

    TTF_Font* font; 
    TTF_Font* font_big;
};

#endif /* __RENDER_H__ */
