#include "graphics.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define SCREEN_BPP 32
#define SDL_DEFAULT_REPEAT_DELAY 500
#define SDL_DEFAULT_REPEAT_INTERVAL 30

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

SDL_Rect getSourceRectangleBlockTextures(enum Block_e block, unsigned int level);

// Init SDL2
void initGraphics(SDL_Window** window, SDL_Renderer** renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("SDL initialized!\n");
    }
    
    // Create window
    *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("Window created!\n");
    }
    
    // Create renderer for window
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("Renderer created!\n");
    }
    
    // Initialize SDL_image
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        exit(1);
    }
    else
    {
        printf("SDL_image initialized!\n");
    }

}

SDL_Surface* loadBlocksSprite(const char* path)
{
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        exit(1);
    }
    else
    {
        printf("Image %s loaded!\n", path);
    }
    return loadedSurface;
}

SDL_Surface* loadCharactersSprite(const char* path)
{
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        exit(1);
    }
    else
    {
        printf("Image %s loaded!\n", path);
    }
    return loadedSurface;
}

SDL_Texture* createCharactersTexture(SDL_Renderer* renderer, SDL_Surface* sprite)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sprite);
    if (texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", "characters.png", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("Texture created from %s!\n", "characters.png");
    }
    return texture;
}

SDL_Rect getSourceRectangleCharactersTexture(char character)
{
    const char* characters_digits = "1234567890";
    const char* characters_AG = "ABCDEFG";
    const char* characters_HS = "HIJKLMNOPQRS";
    const char* characters_TZ = "TUVWXYZ";
    char* characters;

    unsigned int idx = 0;
        SDL_Rect sourceRect;

    // if charachter is a digit
    if (character >= '0' && character <= '9')
    {
        sourceRect.x = 8;
        sourceRect.y = 0;
        characters = characters_digits;
    }
    else if(character >= 'A' && character <= 'G')
    {
        sourceRect.x = 40;
        sourceRect.y = 24;
        characters = characters_AG;
    }
    else if(character >= 'H' && character <= 'S')
    {
        sourceRect.x = 0;
        sourceRect.y = 32;
        characters = characters_HS;
    }
    else if(character >= 'T' && character <= 'Z')
    {
        sourceRect.x = 0;
        sourceRect.y = 40;
        characters = characters_TZ;
    }
    while(characters[idx] != '\0' && characters[idx] != character)
        idx++;
    sourceRect.x = sourceRect.x + idx*8;
    sourceRect.w = 8;
    sourceRect.h = 8;

    return sourceRect;
}

SDL_Surface* loadPlayGround(const char* path)
{
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        exit(1);
    }
    else
    {
        printf("Image %s loaded!\n", path);
    }
    return loadedSurface;
}


// Load the entire row of blocks into a surface
SDL_Surface* createBlockSurfaceFromSprite(SDL_Surface* sprite, unsigned int level)
{
    const unsigned int block_width = sprite->w, block_height = sprite->h;
    SDL_Surface* blockSurface = SDL_CreateRGBSurface(0, block_width*4, block_height, SCREEN_BPP, 0, 0, 0, 0);
    if (blockSurface == NULL)
    {
        printf("Unable to create surface! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("Surface created!\n");
    }
    SDL_Rect sourceRect;
    sourceRect.x = 0;
    sourceRect.y = level*block_height;
    sourceRect.w = block_width;
    sourceRect.h = block_height;
    SDL_BlitSurface(sprite, &sourceRect, blockSurface, NULL);
    return sprite;
}

SDL_Texture* createBlockTextures(SDL_Renderer* renderer, SDL_Surface* sprite, unsigned int level)
{
    SDL_Texture* blockTextures = SDL_CreateTextureFromSurface(renderer, createBlockSurfaceFromSprite(sprite, level));
    
        if (blockTextures == NULL)
        {
            printf("Unable to create texture! SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
        else
        {
            printf("Texture created!\n");
        }

    return blockTextures;
}

SDL_Surface* createCharactersSurfaceFromSprite(SDL_Surface* sprite, char character)
{
    const unsigned int character_width = sprite->w/12, character_height = sprite->h/14;
    SDL_Surface* character_surface = SDL_CreateRGBSurface(0, character_width, character_height, SCREEN_BPP, 0, 0, 0, 0);
    if (character_surface == NULL)
    {
        printf("Unable to create surface! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    else
    {
        printf("Surface created!\n");
    }

    const char* characters = "1234567890";
    unsigned int idx = 0;
    while(characters[idx] != '\0' && characters[idx] != character)
        idx++;
    
    unsigned int row = idx/16;
    unsigned int col = idx%16;

    SDL_Rect sourceRect;
    sourceRect.x = col*character_width;//block * block_width;
    sourceRect.y = row*character_height;
    sourceRect.w = character_width;
    sourceRect.h = character_height;
    SDL_BlitSurface(sprite, &sourceRect, character_surface, NULL);

    return character_surface;
}

void renderCharacterTextureToZone(SDL_Renderer* renderer, SDL_Texture* characters_texture, char c, enum Zone_e zone, const unsigned int square_idx)
{
    SDL_Rect zone_rectangle;
    SDL_Rect fill_rect;
    switch (zone)
    {
        case STATISTICS:
            break;
        case LINES:
            if(square_idx == 0)
                zone_rectangle = (SDL_Rect){275, 19, 41, 9};
            else if(square_idx == 1)
                zone_rectangle = (SDL_Rect){291, 19, 41, 9};
            else
                zone_rectangle = (SDL_Rect){307, 19, 41, 9};
            break;
        case TOP:
            if(square_idx == 0)
                zone_rectangle = (SDL_Rect){352, 40, 41, 9};
            else if(square_idx == 1)
                zone_rectangle = (SDL_Rect){367, 40, 41, 9};
            else if(square_idx == 2)
                zone_rectangle = (SDL_Rect){382, 40, 41, 9};
            else if(square_idx == 3)
                zone_rectangle = (SDL_Rect){397, 40, 41, 9};
            else if(square_idx == 4)
                zone_rectangle = (SDL_Rect){412, 40, 41, 9};
            else if(square_idx == 5)
                zone_rectangle = (SDL_Rect){427, 40, 41, 9};
            fill_rect = (SDL_Rect){352, 40, 427-352, 9};
            break;
        case SCORE:
            if(square_idx == 0)
                zone_rectangle = (SDL_Rect){352, 69, 41, 9};
            else if(square_idx == 1)
                zone_rectangle = (SDL_Rect){367, 69, 41, 9};
            else if(square_idx == 2)
                zone_rectangle = (SDL_Rect){382, 69, 41, 9};
            else if(square_idx == 3)
                zone_rectangle = (SDL_Rect){397, 69, 41, 9};
            else if(square_idx == 4)
                zone_rectangle = (SDL_Rect){412, 69, 41, 9};
            else if(square_idx == 5)
                zone_rectangle = (SDL_Rect){427, 69, 41, 9};
            break;
        case NEXT:
            break;
        case LEVEL:
            if(square_idx == 0)
                zone_rectangle = (SDL_Rect){382, 195, 41, 9};
            else if(square_idx == 1)
                zone_rectangle = (SDL_Rect){397, 195, 41, 9};
            break;
        
        default:
            break;
    }

    SDL_Rect source_rect = getSourceRectangleCharactersTexture(c);
    SDL_RenderCopy(renderer, characters_texture, &source_rect, &zone_rectangle);
}

void renderLine(SDL_Renderer* renderer, SDL_Texture* characters_texture, unsigned int line)
{
     // Get the number of digits in top_score
    unsigned int digits = 0;
    unsigned int tmp = line;
    while(tmp > 0)
    {
        tmp /= 10;
        digits++;
    }
    unsigned int top_score_digits[3] = {0};
    unsigned int top_score_digits_idx = 2;
    while(line > 0)
    {
        top_score_digits[top_score_digits_idx] = line%10;
        top_score_digits_idx--;
        line /= 10;
    }

    char c;
    for(unsigned int i = 0; i < 3; i++)
    {
        c = '0' + top_score_digits[i];;
        renderCharacterTextureToZone(renderer, characters_texture, c, LINES, i);
    }
}

void renderTop(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int top_score)
{   
    // Get the number of digits in top_score
    unsigned int digits = 0;
    unsigned int tmp = top_score;
    while(tmp > 0)
    {
        tmp /= 10;
        digits++;
    }
    unsigned int top_score_digits[6] = {0};
    unsigned int top_score_digits_idx = 5;
    while(top_score > 0)
    {
        top_score_digits[top_score_digits_idx] = top_score%10;
        top_score_digits_idx--;
        top_score /= 10;
    }

    // Loop over the digits and render them
    for(unsigned int i = 0; i < 6; i++)
    {
        char c = '0' + top_score_digits[i];
        renderCharacterTextureToZone(renderer, charachters_texture, c, TOP, i);
    }
}

void renderScore(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int score)
{
    // Get the number of digits in score
    unsigned int digits = 0;
    unsigned int tmp = score;
    while(tmp > 0)
    {
        tmp /= 10;
        digits++;
    }
    // Split score into digits and store it in a array
    unsigned int score_digits[6] = {0};
    unsigned int score_digits_idx = 5;
    while(score > 0)
    {
        score_digits[score_digits_idx] = score%10;
        score_digits_idx--;
        score /= 10;
    }
    // Loop over the digits and render them
    for(unsigned int i = 0; i < 6; i++)
    {
        char c = '0' + score_digits[i];
        renderCharacterTextureToZone(renderer, charachters_texture, c, SCORE, i);
    }
}

void renderCharacterToStatistics(SDL_Renderer* renderer, SDL_Texture* characters_texture, char c, enum Tetromino_e type, unsigned int idx)
{
    SDL_Rect zone_rectangle;
    switch (type)
    {
        case T:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 107, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 107, 41, 8};
            else
                zone_rectangle = (SDL_Rect){111, 107, 41, 8};
            break;
        case J:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 126, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 126, 41, 8};
            else if(idx == 2)
                zone_rectangle = (SDL_Rect){111, 126, 41, 8};
            break;
        case Z:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 145, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 145, 41, 8};
            else
                zone_rectangle = (SDL_Rect){111, 145, 41, 8};
            break;
        case O:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 165, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 165, 41, 8};
            else
                zone_rectangle = (SDL_Rect){111, 165, 41, 8};
            break;
        case S:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 185, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 185, 41, 8};
            else
                zone_rectangle = (SDL_Rect){111, 185, 41, 8};
            break;
        case L:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 204, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 204, 41, 8};
            else if(idx == 2)
                zone_rectangle = (SDL_Rect){111, 204, 41, 8};
            else
                zone_rectangle = (SDL_Rect){126, 204, 41, 8};
            break;
        case I:
            if(idx == 0)
                zone_rectangle = (SDL_Rect){81, 223, 41, 8};
            else if(idx == 1)
                zone_rectangle = (SDL_Rect){96, 223, 41, 8};
            else if(idx == 2)
                zone_rectangle = (SDL_Rect){111, 223, 41, 8};
            else
                zone_rectangle = (SDL_Rect){126, 223, 41, 8};
            break;
    }

    SDL_Rect source_rect = getSourceRectangleCharactersTexture(c);
    SDL_RenderCopy(renderer, characters_texture, &source_rect, &zone_rectangle);
}

void renderStatistics(SDL_Renderer* renderer, SDL_Texture* charachters_texture, enum Tetromino_e type, unsigned int number)
{
    unsigned int digits = 0;
    unsigned int tmp = number;
    while(tmp > 0)
    {
        tmp /= 10;
        digits++;
    }
    unsigned int statistics_digits[3] = {0};
    unsigned int statistics_digits_idx = 2;
    while(number > 0)
    {
        statistics_digits[statistics_digits_idx] = number%10;
        statistics_digits_idx--;
        number /= 10;
    }

    // Loop over the digits and render them
    for(unsigned int i = 0; i < 3; i++)
    {
        char c = '0' + statistics_digits[i];
        renderCharacterToStatistics(renderer, charachters_texture, c, type, i);
    }
}

void renderStatisticsTetrominos(SDL_Renderer* renderer, SDL_Texture* blocks_texture, unsigned int level)
{
    static const SDL_Rect board_rect = {374, 48*3-10, 149, 194};
    SDL_Rect block_rect;
    SDL_Rect source_rect_block_textures;

    static const enum Block_e T[] = {ONE, ONE, ONE, NONE, NONE, ONE, NONE, NONE};
    static const SDL_Rect T_rect = (SDL_Rect){47.5, 103, 32, 14};
    
    static const enum Block_e J[] = {THREE, THREE, THREE, NONE, NONE, NONE, THREE, NONE};
    static const SDL_Rect J_rect = (SDL_Rect){47.5, 121, 32, 14};

    static const enum Block_e Z[] = {TWO, TWO, NONE, NONE, NONE, TWO, TWO, NONE};
    static const SDL_Rect Z_rect = (SDL_Rect){47.5, 141, 32, 14};

    static const enum Block_e O[] = {NONE, ONE, ONE, NONE, NONE, ONE, ONE, NONE};
    static const SDL_Rect O_rect = (SDL_Rect){41.5, 161, 32, 14};

    static const enum Block_e S[] = {NONE, THREE, THREE, NONE, THREE, THREE, NONE, NONE};
    static const SDL_Rect S_rect = (SDL_Rect){47.5, 181, 32, 14};

    static const enum Block_e L[] = {TWO, TWO, TWO, NONE, TWO, NONE, NONE, NONE};
    static const SDL_Rect L_rect = (SDL_Rect){47.5, 199, 32, 14};


    static const enum Block_e I[] = {ONE, ONE, ONE, ONE, NONE, NONE, NONE, NONE};
    static const SDL_Rect I_rect = (SDL_Rect){43.5, 223, 32, 14};

    static const enum Block_e* tetrominos[] = {T, J, Z, O, S, L, I};
    static const SDL_Rect tetrominos_rect[] = {T_rect, J_rect, Z_rect, O_rect, S_rect, L_rect, I_rect};
    for (size_t k = 0; k < 7; k++)
    {
        for(unsigned int i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                block_rect = (SDL_Rect){tetrominos_rect[k].x + j*11, tetrominos_rect[k].y + i*7, 11, 7};
                if(tetrominos[k][i*4+j] != NONE)
                {
                    source_rect_block_textures = getSourceRectangleBlockTextures(tetrominos[k][i*4+j], level);
                    SDL_RenderCopy(renderer, blocks_texture, &source_rect_block_textures, &block_rect);
                }
            }
        }  
    }
}

void renderAllStatistics(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int* statistics)
{
    // Render the shapes associated to the statistics
    renderStatistics(renderer, charachters_texture, T, statistics[T]);
    renderStatistics(renderer, charachters_texture, J, statistics[J]);
    renderStatistics(renderer, charachters_texture, Z, statistics[Z]);
    renderStatistics(renderer, charachters_texture, O, statistics[O]);
    renderStatistics(renderer, charachters_texture, S, statistics[S]);
    renderStatistics(renderer, charachters_texture, L, statistics[L]);
    renderStatistics(renderer, charachters_texture, I, statistics[I]);
}

void renderNext(SDL_Renderer* renderer, SDL_Texture* blocks_texture, struct BlockList_s* tetromino, unsigned int level)
{
    if(!tetromino)
        return;
    static const SDL_Rect board_rect = {374, 48*3-10, 149, 194};
    SDL_Rect block_rect;
    SDL_Rect source_rect_block_textures;
    // Create a temporary 3x3 board
    enum Block_e board[8];
    enum Block_e block = tetromino->block;
    
    switch (tetromino->type)
    {
        case T:
            memcpy(board, (enum Block_e[8]){block, block, block, NONE, NONE, block, NONE, NONE}, 8*sizeof(enum Block_e));
            break;
        case J:
            memcpy(board, (enum Block_e[8]){block, block, block, NONE, NONE, NONE, block, NONE}, 8*sizeof(enum Block_e));
            break;
        case Z:
            memcpy(board, (enum Block_e[8]){block, block, NONE, NONE, NONE, block, block, NONE}, 8*sizeof(enum Block_e));
            break;
        case O:
            memcpy(board, (enum Block_e[8]){NONE, block, block, NONE, NONE, block, block, NONE}, 8*sizeof(enum Block_e));
            break;
        case S:
            memcpy(board, (enum Block_e[8]){NONE, block, block, NONE, block, block, NONE, NONE}, 8*sizeof(enum Block_e));
            break;
        case L:
            memcpy(board, (enum Block_e[8]){block, block, block, NONE, block, NONE, NONE, NONE}, 8*sizeof(enum Block_e));
            break;
        case I:
            memcpy(board, (enum Block_e[8]){block, block, block, block, NONE, NONE, NONE, NONE}, 8*sizeof(enum Block_e));
            break;

    } 

    // Render the board 
    
    for(unsigned int i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            block_rect = (SDL_Rect){board_rect.x + j*9, board_rect.y + i*9, 9, 9};
            source_rect_block_textures = getSourceRectangleBlockTextures(board[i*4 + j], level);
            if(board[i*4+j] != NONE)
                SDL_RenderCopy(renderer, blocks_texture, &source_rect_block_textures, &block_rect);
        }
    }
}

void renderLevel(SDL_Renderer* renderer, SDL_Texture* charachters_texture, unsigned int level)
{
    // Get the number of digits in level
    unsigned int digits = 0;
    unsigned int tmp = level;
    while(tmp > 0)
    {
        tmp /= 10;
        digits++;
    }
    unsigned int level_digits[2] = {0};
    unsigned int level_digits_idx = 1;
    while(level > 0)
    {
        level_digits[level_digits_idx] = level%10;
        level_digits_idx--;
        level /= 10;
    }

    // Loop over the digits and render them
    for(unsigned int i = 0; i < 2; i++)
    {
        char c = '0' + level_digits[i];
        renderCharacterTextureToZone(renderer, charachters_texture, c, LEVEL, i);
    }
}

SDL_Rect getRectangleFromMouseClicks(unsigned int prev_x, unsigned int prev_y, unsigned int x, unsigned int y)
{
    SDL_Rect rect = {prev_x, prev_y, x-prev_x, y-prev_y};
    return rect;
}

SDL_Rect getSourceRectangleBlockTextures(enum Block_e block, unsigned int level)
{
    SDL_Rect sourceRect;
    switch (block)
    {
         case ONE:
            sourceRect.x = 0;
            sourceRect.y = ((level%10) * 8);
            sourceRect.w = 8;
            sourceRect.h = 8;
            break;
        case TWO:
            sourceRect.x = 8;
            sourceRect.y = ((level%10) * 8);
            sourceRect.w = 8;
            sourceRect.h = 8;
            break;
        case THREE:
            sourceRect.x = 16;
            sourceRect.y = ((level%10) * 8);
            sourceRect.w = 8;
            sourceRect.h = 8;
            break;
        case FOUR:
            sourceRect.x = 24;
            sourceRect.y = ((level%10) * 8);
            sourceRect.w = 8;
            sourceRect.h = 8;
            break;
    }
    //printf("%d\n", level);
    return sourceRect;
}

void renderBoard(SDL_Renderer* renderer, enum Block_e* board, SDL_Texture* block_textures, unsigned int level)
{
    static const SDL_Rect board_rect = {177, 28, 152, 197};
    SDL_Rect block_rect;
    SDL_Rect source_rect_block_textures;
    for (size_t i = 2; i < 22; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            block_rect = (SDL_Rect){board_rect.x + j*15.200000, board_rect.y + i*9.850000, 15.200000, 9.850000};
            source_rect_block_textures = getSourceRectangleBlockTextures(board[i*gNumberCols + j], level);
            if(board[i*gNumberCols+j] != NONE){
                SDL_RenderCopy(renderer, block_textures, &source_rect_block_textures, &block_rect);
            }
        }
        
    }
}

void renderLineAnimation(SDL_Renderer* renderer, SDL_Texture* block_textures, enum Block_e* board, unsigned int* line_to_destroy, int nb_lines_to_destroy, unsigned int level)
{
    // Destroy each line blocks starting at the center
    for(unsigned int i = 4, j = 5; i >= 0; i--, j++)
    {
       for (size_t k = 0; k < nb_lines_to_destroy; k++)
       {
            board[line_to_destroy[k]*gNumberCols + j]= NONE;
            board[line_to_destroy[k]*gNumberCols + i]= NONE;
       }
       renderBoard(renderer, board, block_textures, level);
       SDL_RenderPresent(renderer);
       SDL_Delay(10);
    }
}

SDL_Texture* loadTitleSceneTexture(SDL_Renderer* renderer)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load("gfx/title.png");
    if(surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", "assets/title.png", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", "assets/title.png", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* loadMenuSceneTexture(SDL_Renderer* renderer)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load("gfx/menu.png");
    if(surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", "assets/menu.png", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", "assets/menu.png", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}


SDL_Texture* loadMenuASceneTexture(SDL_Renderer* renderer)
{   
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load("gfx/menu_a.png");
    if(surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", "assets/menu_a.png", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", "assets/menu_a.png", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}

void changeBlackPixelFromSurface(SDL_Surface* surface)
{
    for(int i = 0; i < surface->h; i++)
    {
        for(int j = 0; j < surface->w; j++)
        {
            //setPixel(surface, i, j, 0x3898fcfff);
            
            Uint32 pixel = getPixel(surface, i, j);
            if((pixel == 0x00000000) || (pixel == 0x410b00))
            {
                setPixel(surface, i, j, 0xfc9838);
            }
        }   
    }
}

void changeOrangePixelFromSurface(SDL_Surface* surface)
{
    for(int i = 0; i < 48; i++)
    {
        for(int j = 0; j < 48; j++)
        {
            Uint32 pixel = getPixel(surface, i, j);
            if(pixel == 0xfffc9838)
            {
                setPixel(surface, i, j, 0x00000000);
            }
        }   
    }
}

void renderMenuScene(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Texture** loadASceneTextures(SDL_Renderer* renderer)
{
    SDL_Texture** textures = malloc(sizeof(SDL_Texture*) * 5);
    
    SDL_Surface* blocks_sprite = loadBlocksSprite("gfx/blocks_sprite.png");
    SDL_Surface* characters_sprite = loadCharactersSprite("gfx/characters.png");
    SDL_Surface* playground = loadPlayGround("gfx/playground.png");
    SDL_Surface* socred_playground = loadPlayGround("gfx/scored_playground.png");
    SDL_Surface* master = IMG_Load("gfx/master_a.png");

    textures[PLAYGROUND] = SDL_CreateTextureFromSurface(renderer, playground);
    textures[SCORED_PLAYGROUND] = SDL_CreateTextureFromSurface(renderer, socred_playground);
    textures[BLOCKS] = SDL_CreateTextureFromSurface(renderer, blocks_sprite);
    textures[CHARACTERS] = SDL_CreateTextureFromSurface(renderer, characters_sprite);
    textures[MASTER] = SDL_CreateTextureFromSurface(renderer, master);
    
    SDL_FreeSurface(blocks_sprite);
    SDL_FreeSurface(characters_sprite);
    SDL_FreeSurface(playground);
    SDL_FreeSurface(socred_playground);
    SDL_FreeSurface(master);
    
    return textures;
}

void renderTitleScene(SDL_Renderer* renderer, SDL_Texture* title_texture)
{
    SDL_Rect title_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
    SDL_RenderPresent(renderer);
}

// Render the entiere game scene
void renderAScene(SDL_Renderer* renderer, SDL_Texture** scene_textures, struct GameState_s* game_state)
{
        SDL_RenderCopy(renderer, scene_textures[PLAYGROUND], NULL, NULL);
        renderTop(renderer, scene_textures[CHARACTERS], game_state->top);
        renderLine(renderer, scene_textures[CHARACTERS], game_state->lines);
        renderScore(renderer, scene_textures[CHARACTERS], game_state->score);
        renderLevel(renderer, scene_textures[CHARACTERS], game_state->level);
        renderNext(renderer, scene_textures[BLOCKS], game_state->next_tetromino, game_state->level);
        renderStatisticsTetrominos(renderer, scene_textures[BLOCKS], game_state->level);
        renderAllStatistics(renderer, scene_textures[CHARACTERS], game_state->statistics);
        renderBoard(renderer, game_state->board, scene_textures[BLOCKS], game_state->level);
        SDL_RenderPresent(renderer);
}

SDL_Texture* loadMasterASceneTexture(SDL_Renderer* renderer)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load("gfx/master_a.png");
    if(surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", "assets/master_a.png", IMG_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", "assets/master_a.png", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}

void renderCharacterTextureToName(SDL_Renderer* renderer, SDL_Texture* character_texture, size_t name_idx, size_t char_idx, char c, int selected)
{
    SDL_Rect rect_dest = {134, 184 + 18 * name_idx, 20, 10};
    rect_dest.x = rect_dest.x + (char_idx * 15);
    // Is char in the alphabet?
   if(selected){
            SDL_SetRenderDrawColor(renderer, 0xfc, 0x98, 0x38, 0xFF);
            rect_dest.x = 136 + (char_idx * 15);
            rect_dest.w = 13;
            SDL_RenderFillRect(renderer, &rect_dest);
            rect_dest.x = 134 + (char_idx * 15);
            rect_dest.w = 20;
        }
    else{
        if(!selected){
            rect_dest.w = 13;
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &rect_dest);
            rect_dest.w = 20;
        }
    }
        SDL_Rect character_source_rect = getSourceRectangleCharactersTexture(c);
        SDL_RenderCopy(renderer, character_texture, &character_source_rect, &rect_dest);
}

void renderCharacterTextureToScore(SDL_Renderer* renderer, SDL_Texture* character_texture, size_t name_idx, size_t char_idx, char c, int selected)
{
    SDL_Rect rect_dest = {232, 183 + 19 * name_idx, 33, 12};
    rect_dest.x = rect_dest.x + (char_idx * 15);
    SDL_Rect character_source_rect = getSourceRectangleCharactersTexture(c);
    SDL_RenderCopy(renderer, character_texture, &character_source_rect, &rect_dest);
}

void renderCharacterTextureToLevel(SDL_Renderer* renderer, SDL_Texture* character_texture, size_t name_idx, size_t char_idx, char c, int selected)
{
    SDL_Rect rect_dest = {337, 185 + 19 * name_idx, 33, 10};
    rect_dest.x = rect_dest.x + (char_idx * 15);
    SDL_Rect character_source_rect = getSourceRectangleCharactersTexture(c);
    SDL_RenderCopy(renderer, character_texture, &character_source_rect, &rect_dest);
}


void renderPlayerName(SDL_Renderer* renderer, SDL_Texture* characters_texture, size_t name_idx, char* name)
{
    for(size_t i = 0; name[i] != '\0'; i++)
    {
        renderCharacterTextureToName(renderer, characters_texture, name_idx, i, name[i], 0);
    }   
}

void renderPlayerScore(SDL_Renderer* renderer, SDL_Texture* characters_texture, size_t name_idx, char* score)
{
    for(size_t i = 0; score[i] != '\0'; i++)
    {
        renderCharacterTextureToScore(renderer, characters_texture, name_idx, i, score[i], 0);
    } 
}

void renderPlayersLevels(SDL_Renderer* renderer, SDL_Texture* characters_texture, size_t name_idx, char* level)
{
    for(size_t i = 0; level[i] != '\0'; i++)
    {
        renderCharacterTextureToLevel(renderer, characters_texture, name_idx, i, level[i], 0);
    } 
}
