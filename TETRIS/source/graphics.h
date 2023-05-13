#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

enum Scene_e {TITLE, MENU, MENU_A, GAME_A, MASTER_A};

enum Block_e {NONE, ONE, TWO, THREE, FOUR};
enum Zone_e {STATISTICS, LINES, TOP, SCORE, NEXT, LEVEL};
enum Tetromino_e {I, O, T, L, J, Z, S};

struct BlockList_s{
    enum Tetromino_e type;
    enum Block_e block;
    struct BlockNode_s* blocks;
    struct BlockNode_s* last_block;
    struct BlockNode_s* pivot_block;
};

struct BlockNode_s{
    enum Block_e** block;
    struct BlockNode_s* next_block;
    struct BlockNode_s* previous_block;
};

struct GameState_s{
    unsigned int* statistics;
    unsigned int lines;
    unsigned int top;
    unsigned int score;
    unsigned int level;
    
    enum Block_e* board;
    struct BlockList_s* current_tetromino;
    struct BlockList_s* next_tetromino;
};

enum {PLAYGROUND, SCORED_PLAYGROUND, CHARACTERS, BLOCKS, MASTER};

extern const unsigned int gNumberCols;
extern const unsigned int gNumberRows;

void initGraphics(SDL_Window** window, SDL_Renderer** renderer);
SDL_Surface* loadBlocksSprite(const char* path);
SDL_Surface* loadCharactersSprite(const char* path);
SDL_Surface* createBlockSurfaceFromSprite(SDL_Surface* sprite, unsigned int level);
SDL_Surface* createCharacterSurfaceFromSprite(SDL_Surface* sprite, char character);
SDL_Surface* loadPlayGround(const char* path);
void renderCharacterTextureToZone(SDL_Renderer* renderer, SDL_Texture* character_texture, char c, enum Zone_e zone, const unsigned int square_idx);
SDL_Rect getRectangleFromMouseClicks(unsigned int prev_x, unsigned int prev_y, unsigned int x, unsigned int y);
SDL_Texture* createBlockTextures(SDL_Renderer* renderer, SDL_Surface* sprite, unsigned int level);
SDL_Texture* createCharactersTexture(SDL_Renderer* renderer, SDL_Surface* sprite);
void renderTop(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int top_score);
void renderScore(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int score);
void renderLine(SDL_Renderer* renderer, SDL_Texture* characters_texture, unsigned int line);
void renderLevel(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int level);
void renderNext(SDL_Renderer* renderer, SDL_Texture* blocks_texture, struct BlockList_s* tetromino, unsigned int level);
void renderBoard(SDL_Renderer* renderer, enum Block_e* board, SDL_Texture* block_textures, unsigned int level);
void renderStatistics(SDL_Renderer* renderer, SDL_Texture* charachters_texture, enum Tetromino_e type, unsigned int number);
void renderAllStatistics(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int* statistics);
void renderLineAnimation(SDL_Renderer* renderer, SDL_Texture* block_textures, enum Block_e* board, unsigned int* line_to_destroy, int nb_lines_to_destroy, unsigned int level);
void renderStatisticsTetrominos(SDL_Renderer* renderer, SDL_Texture* blocks_texture, unsigned int level);
void renderAScene(SDL_Renderer* renderer, SDL_Texture** scene_textures, struct GameState_s* game_state);
SDL_Texture** loadASceneTextures(SDL_Renderer* renderer);
SDL_Texture* loadTitleSceneTexture(SDL_Renderer* renderer);
SDL_Texture* loadMenuSceneTexture(SDL_Renderer* renderer);
void renderMenuScene(SDL_Renderer* renderer, SDL_Texture* texture);
void renderTitleScene(SDL_Renderer* renderer, SDL_Texture* title_texture);
SDL_Texture* loadMenuASceneTexture(SDL_Renderer* renderer);
void changeBlackPixelFromSurface(SDL_Surface* surface);
void changeOrangePixelFromSurface(SDL_Surface* surface);
void renderEndBoardAnimation(SDL_Renderer* renderer, SDL_Texture* blocks_textures, struct GameState_s* game_state);
SDL_Texture* loadMasterASceneTexture(SDL_Renderer* renderer);
SDL_Rect getSourceRectangleCharactersTexture(char character);
void renderCharacterTextureToName(SDL_Renderer* renderer, SDL_Texture* character_texture, size_t name_idx, size_t char_idx, char c, int selected);
void renderPlayerName(SDL_Renderer* renderer, SDL_Texture* characters_texture, size_t name_idx, char* name);
void renderPlayerScore(SDL_Renderer* renderer, SDL_Texture* characters_texture, size_t name_idx, char* score);
void renderPlayersLevels(SDL_Renderer* renderer, SDL_Texture* characters_texture, size_t name_idx, char* level);
#endif // DEBUG
