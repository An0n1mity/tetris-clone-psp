#include "scenes.h"

enum Scene_e menuSceneLoop(SDL_Renderer* renderer, SDL_Texture* menu_texture, int* game_type, OSL_SOUND* musics[3])
{
    unsigned int nb_frames = 0;

    SDL_RenderCopy(renderer, menu_texture, NULL, NULL);
    static const SDL_Rect a_type_rect = {104, 58, 148, 28};
    static const SDL_Rect b_type_rect = {284, 58, 150, 28};
    static const SDL_Rect music_1_rect = {185, 155, 165, 28};
    static const SDL_Rect music_2_rect = {185, 175, 165, 28};
    static const SDL_Rect music_3_rect = {185, 195, 165, 28};
    static const SDL_Rect music_off_rect = {215, 215, 87, 28};

    static const SDL_Rect* game_type_rects[2] = {&a_type_rect, &b_type_rect};
    static const SDL_Rect* music_type_rects[4] = {&music_1_rect, &music_2_rect, &music_3_rect, &music_off_rect};

    SDL_Color red = {255, 0, 0, 255};
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 255);

    static SDL_Rect trigon_rect = a_type_rect;
    filledTrigonColor(renderer, trigon_rect.x + 10, trigon_rect.y + 10, trigon_rect.x + 10, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + 20, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
    filledTrigonColor(renderer, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + 10, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + trigon_rect.w - 18, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
    
    SDL_RenderPresent(renderer);
    int mouse_x, mouse_y, previous_x, previous_y;
    int click = 0;
    SDL_Event event;

    int game_type_selection = 0;
    int music_type_selection = 1;
    unsigned int cursor_row = 0, cursor_col = 0;
    unsigned int game_type_cursor = 0;
    unsigned int music_type_cursor = 0;
    oslPlaySound(musics[music_type_cursor], 0);

    int music_selected = 0;
    int game_selected = 0;

    while(1)
    {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;
        if(event.type == SDL_JOYBUTTONDOWN)
        {
            oslPlaySound(g_select_sfx, 1);
            if(event.jbutton.button == 9 && (!game_selected))
                if(game_type_cursor < 1)
                    game_type_cursor++;
            if(event.jbutton.button == 7 && (!game_selected))
                if(game_type_cursor >= 1)
                    game_type_cursor--;
            if((event.jbutton.button == 6) && (!music_selected) && (game_selected)){
                if(music_type_cursor < 3){
                    music_type_cursor++;
                    if(music_type_cursor == 3)
                        ;//Mix_HaltMusic();
                    else{
                        oslPlaySound(musics[music_type_cursor], 0);
                    }
                }
            }
            if(event.jbutton.button == 8  && (!music_selected) && (game_selected)){
                if(music_type_cursor >= 1){
                    music_type_cursor--;
                    oslPlaySound(musics[music_type_cursor], 0);
                }
            }
            if(event.jbutton.button == 2){
                if(!game_selected)
                    game_selected = 1;
                else if(game_selected && (!music_selected))
                    music_selected = 1;
                else if(game_selected && music_selected)
                    return MENU_A;
                
            }         
            else if(event.jbutton.button == 1){
                if(game_selected && music_selected)
                    music_selected = 0;
                else if(game_selected)
                    game_selected = 0;
            }

        }

        // Draw the cursor for the game type and music
        SDL_RenderCopy(renderer, menu_texture, NULL, NULL);
        if(music_selected)
        {
            trigon_rect = *music_type_rects[music_type_cursor];
            filledTrigonColor(renderer, trigon_rect.x + 10, trigon_rect.y + 10, trigon_rect.x + 10, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + 20, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
            filledTrigonColor(renderer, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + 10, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + trigon_rect.w - 18, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
        }
        if(game_selected)
        {
            trigon_rect = *game_type_rects[game_type_cursor];
            filledTrigonColor(renderer, trigon_rect.x + 10, trigon_rect.y + 10, trigon_rect.x + 10, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + 20, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
            filledTrigonColor(renderer, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + 10, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + trigon_rect.w - 18, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
        }
        if(!(nb_frames % 5))
        {
            if(!game_selected)
            {
                trigon_rect = *game_type_rects[game_type_cursor];
                filledTrigonColor(renderer, trigon_rect.x + 10, trigon_rect.y + 10, trigon_rect.x + 10, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + 20, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
                filledTrigonColor(renderer, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + 10, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + trigon_rect.w - 18, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
            }
            if(!music_selected)
            {
                trigon_rect = *music_type_rects[music_type_cursor];
                filledTrigonColor(renderer, trigon_rect.x + 10, trigon_rect.y + 10, trigon_rect.x + 10, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + 20, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
                filledTrigonColor(renderer, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + 10, trigon_rect.x + trigon_rect.w - 8, trigon_rect.y + trigon_rect.h - 10, trigon_rect.x + trigon_rect.w - 18, trigon_rect.y + trigon_rect.h/2, 0xFFFFFFFF);
            }
        } 
        SDL_RenderPresent(renderer);

        nb_frames++;

        if(nb_frames >= 60)
            nb_frames = 0;
    }
}

enum Scene_e menuASceneLoop(SDL_Renderer* renderer, SDL_Texture** menu_a_texture, SDL_Surface* menu_a_surface, unsigned int* level)
{
    SDL_Rect level_rect = {100, 93, 25, 55};
    SDL_Surface* cropped_surface = SDL_CreateRGBSurface(0, level_rect.w, level_rect.y, 32, 0, 0, 0, 0);
    for(int i = 0; i < cropped_surface->h; i++)
    {
        for(int j = 0; j < cropped_surface->w; j++)
        {
            Uint32 pixel = getPixel(cropped_surface, i, j); 
            if((pixel != 0xd82800) && (pixel != 0x000000))
            {
                setPixel(cropped_surface, i, j, 0xd82800);
            }

        }
    }

    SDL_BlitScaled(menu_a_surface, &level_rect, cropped_surface, NULL);
    SDL_Texture* cropped_texture = SDL_CreateTextureFromSurface(renderer, cropped_surface);
    SDL_RenderCopy(renderer, *menu_a_texture, NULL, NULL);
    SDL_RenderCopy(renderer, cropped_texture, NULL, &level_rect);
    SDL_DestroyTexture(cropped_texture);
    SDL_RenderPresent(renderer);
    
    unsigned int cursor_row = 0, cursor_col = 0;
    unsigned int nb_frames = 0;
    SDL_Event event;

    while(1)
    {

        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;
        if(event.type == SDL_JOYBUTTONDOWN)
        {
            oslPlaySound(g_select_sfx, 1); 
            if(event.jbutton.button == 9)
                if(cursor_col < 4)
                    cursor_col++;
            if(event.jbutton.button == 7)
                if(cursor_col >= 1)
                    cursor_col--;
            if(event.jbutton.button == 6)
                if(cursor_row < 1)
                    cursor_row++;
            if(event.jbutton.button == 8)
                if(cursor_row >= 1)
                    cursor_row--;
            if(event.jbutton.button == 2)
            {
                *level = cursor_row * 4 + cursor_col;
                return GAME_A;
            }
            else if(event.jbutton.button == 1)
            {
                return MENU_A;
            }
            
        }
        level_rect.x = 100 + 30 * cursor_col;
        level_rect.y = 93 + 20 * cursor_row;

        SDL_RenderCopy(renderer, *menu_a_texture, NULL, NULL);
        if(!(nb_frames%5))
        {
            SDL_BlitScaled(menu_a_surface, &level_rect, cropped_surface, NULL);
            for(int i = 0; i < cropped_surface->h; i++)
            {
                for(int j = 0; j < cropped_surface->w; j++)
                {
                    Uint32 pixel = getPixel(cropped_surface, i, j); 
                    if((pixel != 0xd82800) && (pixel != 0x000000))
                    {
                        setPixel(cropped_surface, i, j, 0xd82800);
                    }

                }
            }
            changeBlackPixelFromSurface(cropped_surface);
            cropped_texture = SDL_CreateTextureFromSurface(renderer, cropped_surface);
            SDL_RenderCopy(renderer, cropped_texture, NULL, &level_rect);
            SDL_DestroyTexture(cropped_texture);
        }
        SDL_RenderPresent(renderer);

        nb_frames++;
        if(nb_frames >= 60)
            nb_frames = 0;
        
    }

}


char** getPlayersNames()
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    char** players_names = calloc(3, sizeof(char*));
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_name_in_file = strtok(line, " ");
                players_names[idx] = calloc(6, sizeof(char));
                strcpy(players_names[idx], player_name_in_file);
                idx++;
            }
        }
    }
    fclose(file);
    return players_names;
}

char** getPlayersScores()
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    char** players_scores = calloc(3, sizeof(char*));
    for (size_t i = 0; i < 3; i++)
    {
        players_scores[i] = malloc(sizeof(char) * 7);
        for (size_t j = 0; j < 6; j++)
        {
            players_scores[i][j] = '0';
        }
        
    }
    
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_score_in_file = strtok(line, " ");
                player_score_in_file = strtok(NULL,  " ");
                strcpy(players_scores[idx] + (6 - strlen(player_score_in_file)), player_score_in_file);
                idx++;
            }
        }
    }
    fclose(file);
    return players_scores;
}

char** getPlayersLevels()
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    char** players_levels = calloc(3, sizeof(char*));
    for (size_t i = 0; i < 3; i++)
    {
        players_levels[i] = malloc(sizeof(char) * 3);
        for (size_t j = 0; j < 2; j++)
        {
            players_levels[i][j] = '0';
        }
        
    }
    
    size_t idx = 0;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_level_in_file = strtok(line, " ");
                player_level_in_file = strtok(NULL,  " ");
                player_level_in_file = strtok(NULL, "");
                strcpy(players_levels[idx] + (2 - strlen(player_level_in_file)), player_level_in_file);
                idx++;
            }
        }
    }
    fclose(file);

    return players_levels;
}

enum Scene_e masterASceneLoop(SDL_Renderer* renderer, SDL_Surface* master_a_surface, SDL_Texture* a_scene_textures[], struct GameState_s* game_state)
{
    SDL_RenderCopy(renderer, a_scene_textures[MASTER], NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Rect source_rect_master_surface = {72, 152, 8, 8};
    SDL_Rect dest_rect = {135, 184, 18, 10};
    SDL_Surface* cropped_surface = SDL_CreateRGBSurface(0, source_rect_master_surface.w, source_rect_master_surface.h, 32, 0, 0, 0, 0);
   

    int cursor_col = 0, cursor_row = 0, cursor_char = 'A';
    unsigned int nb_frames = 0;
    SDL_Event event;

    unsigned int top_score_place = isScoreTopScore(3000);
    if(top_score_place)
        changeTopScore(3000, game_state->level, 0);
    return;
    char** players_names = getPlayersNames();
    char** players_scores = getPlayersScores();
    char** players_levels = getPlayersLevels();

    while(1){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;
        if(event.type == SDL_JOYBUTTONDOWN)
        {
            if(event.jbutton.button == 1)
                players_names[cursor_row][cursor_col] = 0;
            if(event.jbutton.button == 2){
                if(isalpha(players_names[cursor_row][cursor_col]))
                    return MENU_A;
                players_names[cursor_row][cursor_col] = cursor_char;
            }
            if(event.jbutton.button == 6)
            {
                oslPlaySound(g_select_sfx, 1);
                if(cursor_char > 'A') 
                    cursor_char--;
            }
            if(event.jbutton.button == 8)
            {
                oslPlaySound(g_select_sfx, 1); 
                if(cursor_char < 'Z')
                    cursor_char++;
            }
            if(event.jbutton.button == 9)
                if(cursor_col < 5)
                {
                    oslPlaySound(g_select_sfx, 1); 
                    cursor_col++;
                    if(isalpha(players_names[cursor_row][cursor_col]))
                        cursor_char = players_names[cursor_row][cursor_col];
                    else
                        cursor_char = 'A';
                }
            if(event.jbutton.button == 7)
                if(cursor_col >= 1)
                {
                    oslPlaySound(g_select_sfx, 1); 
                    cursor_col--;
                    if(isalpha(players_names[cursor_row][cursor_col]))
                        cursor_char = players_names[cursor_row][cursor_col];
                    else
                        cursor_char = 'A';
                }
        }
        dest_rect.x = 135 + (cursor_col * 15);
        source_rect_master_surface.x = 72 + (cursor_col * 8);

        SDL_RenderCopy(renderer, a_scene_textures[MASTER], NULL, NULL);

        for (size_t i = 0; i < 3; i++)
        {       
            if(players_names[i]){
                renderPlayerName(renderer, a_scene_textures[CHARACTERS], i, players_names[i]);
                renderPlayerScore(renderer, a_scene_textures[CHARACTERS], i, players_scores[i]);
                renderPlayersLevels(renderer, a_scene_textures[CHARACTERS], i, players_levels[i]);
            }
        }
        

            for(int i = 0; players_names[cursor_row][i] != '\0'; i++)
            {
                if(!isalpha(players_names[cursor_row][i]))
                    continue;
                if((i == cursor_col))
                    renderCharacterTextureToName(renderer,  a_scene_textures[CHARACTERS], cursor_row, i, players_names[cursor_row][i], 1);
                else
                    renderCharacterTextureToName(renderer,  a_scene_textures[CHARACTERS], cursor_row, i, players_names[cursor_row][i], 0);
            }  
        renderCharacterTextureToName(renderer, a_scene_textures[CHARACTERS], 0, cursor_col, cursor_char, 1);

        SDL_RenderPresent(renderer);
        nb_frames++;
        if(nb_frames >= 60)
            nb_frames = 0;
    }
}