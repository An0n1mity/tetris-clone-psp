#ifndef SCENES_H
#define SCENES_H

#include "graphics.h"
#include "sounds.h"

extern OSL_SOUND* g_select_sfx;

enum Scene_e menuSceneLoop(SDL_Renderer* renderer, SDL_Texture* menu_texture, int* game_type, OSL_SOUND* musics[3]);
enum Scene_e menuASceneLoop(SDL_Renderer* renderer, SDL_Texture** menu_a_texture, SDL_Surface* menu_a_surface, unsigned int* level);
enum Scene_e masterASceneLoop(SDL_Renderer* renderer, SDL_Surface* master_a_surface, SDL_Texture* a_scene_textures[], struct GameState_s* game_state);

#endif // 