#define SDL_MAIN_HANDLED 
#include "graphics.h"
#include "scenes.h"
#include "sounds.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <stdio.h>

//enum Block_e {NONE, RED, BLUE, GREEN};


#define VERS    1
#define REVS    1
 
PSP_MODULE_INFO("HelloWorld", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | THREAD_ATTR_VFPU);

static int exitRequest  = 0;

int isRunning()
{
    return !exitRequest;
}
 
int exitCallback(int arg1, int arg2, void *common) 
{ 
    exitRequest = 1; 
    return 0; 
} 
 
/* Callback thread */
int callbackThread(SceSize args, void *argp) 
{ 
	int callbackID;

	callbackID = sceKernelCreateCallback("Exit Callback", exitCallback, NULL); 
	sceKernelRegisterExitCallback(callbackID);

	sceKernelSleepThreadCB();

	return 0; 
}
 
int setupExitCallback() 
{ 
    int threadID = 0; 
 
    threadID = sceKernelCreateThread("Callback Update Thread", callbackThread, 0x11, 0xFA0, 0, 0); 
     
    if(threadID >= 0) 
    { 
        sceKernelStartThread(threadID, 0, 0); 
    } 
 
    return threadID; 
} 


unsigned int getButtonHoldTime(unsigned int push_time)
{
    return SDL_GetTicks() - push_time;
}


const unsigned int gNumberRows = 22;
const unsigned int gNumberCols = 10;
OSL_SOUND* g_select_sfx;

unsigned int getBlockCol(const enum Block_e* const restrict block, const enum Block_e* const restrict board);
unsigned int getBlockRow(const enum Block_e* const restrict block, const enum Block_e* const restrict board);
struct BlockNode_s* createBlockNode(enum Block_e** block); 
struct BlockList_s* createBlockList();

struct BlockList_s* createTetromino(enum Tetromino_e type, enum Block_e block, enum Block_e* anchor_point, enum Block_e* board)
{
    struct BlockList_s* block_list = createBlockList(type, block);
    enum Block_e* ptr = anchor_point;
    struct BlockNode_s* last;
    struct BlockNode_s* pivot;
    switch (type)
    {
    case I:
        for(size_t i = 0; i < 4; i++)
        {
            ptr = anchor_point+i;
            if(*ptr != NONE)
                return NULL;
            if(i == 2)
            {
                pivot = createBlockNode(&ptr);
                addNodeToBlockList(block_list, pivot);
                block_list->pivot_block = pivot; 
            }
            else if(i == 3)
            {                
                last = createBlockNode(&ptr);
                addNodeToBlockList(block_list, last);
                block_list->last_block = last; 
            } 
            else
            {
                addNodeToBlockList(block_list, createBlockNode(&ptr));
            }   

        }
        break;
    case O:
          if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));

        ptr = anchor_point+1;
  if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));

        ptr = anchor_point+gNumberCols;
          if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));

        ptr = anchor_point+gNumberCols+1;
          if(*ptr != NONE)
                return NULL;
        last = createBlockNode(&ptr);
        addNodeToBlockList(block_list, last); 
        block_list->last_block = last;
        break;
    case T:
          if(*ptr != NONE)
                return NULL;
        //*ptr = block;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+1;
          if(*ptr != NONE)
                return NULL;
        //*ptr = block;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+2;
          if(*ptr != NONE)
                return NULL;
        //*ptr = block;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+1+gNumberCols;
          if(*ptr != NONE)
                return NULL;
        pivot = createBlockNode(&ptr);
        block_list->pivot_block = pivot;
        //ptr = block;
        addNodeToBlockList(block_list, pivot);
        block_list->last_block = pivot;
        break;
    case L:
        if(getBlockCol(anchor_point, board) >= gNumberCols-3)
            return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+2+gNumberCols;
          if(*ptr != NONE)
                return NULL;
        last = createBlockNode(&ptr);
        addNodeToBlockList(block_list, last);
        block_list->last_block = last;
        ptr = anchor_point+2;
          if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+1;
          if(*ptr != NONE)
                return NULL;
        pivot = createBlockNode(&ptr);
        addNodeToBlockList(block_list, pivot);
        block_list->pivot_block = pivot;

        break;
     case J:
        if(getBlockCol(anchor_point, board) >= gNumberCols-3)
            return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+1;
          if(*ptr != NONE)
                return NULL;
        pivot = createBlockNode(&ptr);
        addNodeToBlockList(block_list, pivot);
        block_list->pivot_block = pivot;

        ptr = anchor_point+2;
          if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+gNumberCols;
          if(*ptr != NONE)
                return NULL;
        last = createBlockNode(&ptr);
        addNodeToBlockList(block_list, last);
        block_list->last_block = last;
        break;

    case Z:
        if(getBlockCol(anchor_point, board) >= gNumberCols-3)
            return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+1+gNumberCols;
          if(*ptr != NONE)
                return NULL;
        pivot = createBlockNode(&ptr);
        addNodeToBlockList(block_list, pivot);
        block_list->pivot_block = pivot;
        ptr = anchor_point+1;
        if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+gNumberCols+2;
        if(*ptr != NONE)
                return NULL;
        last = createBlockNode(&ptr);
        addNodeToBlockList(block_list, last);
        block_list->last_block = last;
        break;
    case S:
        if(getBlockCol(anchor_point, board) >= gNumberCols-3)
            return NULL;
        ptr = anchor_point+1;
        if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+2;
        if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        ptr = anchor_point+gNumberCols+1;
        if(*ptr != NONE)
                return NULL;
        last = createBlockNode(&ptr);
        addNodeToBlockList(block_list, last);
        block_list->last_block = last;
        block_list->pivot_block = last;
        ptr = anchor_point+gNumberCols;
        if(*ptr != NONE)
                return NULL;
        addNodeToBlockList(block_list, createBlockNode(&ptr));
        break;
    
    default:
        break;
    }
    return block_list;
}

void loadTetrominoToBoard(struct BlockList_s* block_list, enum Block_e* board)
{
    if(!block_list)
        return;
    struct BlockNode_s* ptr = block_list->blocks;
    while(ptr != NULL)
    {
        **(ptr->block) = block_list->block;
        ptr = ptr->next_block;
    }
}

struct BlockList_s* generateRandomTetrimino(enum Block_e* anchor_point, enum Block_e* board)
{
    static unsigned int idx = 0;
    static enum Tetromino_e type[7] = {I, O, T, L, J, Z, S};
    // Randomly shuffle the array
    if(idx == 0)
    {
        for(size_t i = 0; i < 7; i++)
        {
            size_t j = rand() % 7;
            enum Tetromino_e temp = type[i];
            type[i] = type[j];
            type[j] = temp;
        }
    }
    
    enum Block_e block = (enum Block_e)rand()%3 + 1;
    struct BlockList_s* tetromino = createTetromino(type[idx], block, anchor_point, board);
    idx = (idx + 1) % 7;
    return tetromino;
}

struct BlockNode_s* createBlockNode(enum Block_e** block)
{
    struct BlockNode_s* block_list_node = calloc(1, sizeof(struct BlockNode_s));
    block_list_node->block = malloc(sizeof(enum Block_e**));
    *block_list_node->block = *block;
    return block_list_node;
}

struct BlockList_s* createBlockList(enum Tetromino_e type, enum Block_e block)
{
    struct BlockList_s* block_list = calloc(1, sizeof(struct BlockList_s));
    block_list->type = type;
    block_list->block = block;
    return block_list;
}

void addNodeToBlockList(struct BlockList_s* block_list, struct BlockNode_s* block_node)
{
    // If list is empty or block column is equal or less than head node
    if(!block_list->blocks || *block_node->block <= *block_list->blocks->block)
    {
        if(!block_list->blocks){
            block_list->last_block = block_node;
            block_list->blocks = block_node;
            return;
        }
        block_node->next_block = block_list->blocks;
        block_list->blocks->previous_block = block_node;
        block_list->blocks = block_node;
        return;
    }

    // if at the end of the list
    if(*block_list->last_block->block <= *block_node->block)
    {
        block_node->previous_block = block_list->last_block;
        block_list->last_block->next_block = block_node;
        block_list->last_block = block_node;
        return;

    }

    struct BlockNode_s* cursor = block_list->blocks;
    while(*cursor->block < *block_node->block)
    {
        cursor = cursor->next_block;
    }

    cursor->previous_block->next_block = block_node;
    block_node->previous_block = cursor->previous_block;
    cursor->previous_block = block_node;
    block_node->next_block = cursor;

}

const int rightShiftBlockListBlocks(struct BlockNode_s* const blocks, enum Block_e* const board)
{
    if(!blocks)
        return 1;
    
    const unsigned int block_col = getBlockCol(*blocks->block, board);
    const unsigned int block_row = getBlockRow(*blocks->block, board);
    if((block_col == gNumberCols - 1) || *((*blocks->block)+1) != NONE)
        return 0;
    
    // Shift to the right
    const enum Block_e tmp_f = *((*blocks->block));
    const enum Block_e tmp_s = *((*blocks->block)+1);
    
    *((*blocks->block)+1) = *((*blocks->block));
    *((*blocks->block)) = NONE;

    //Update block node
    const enum Block_e* tmp_ptr = *blocks->block;
    *blocks->block = (*blocks->block)+1;
    const int possible = rightShiftBlockListBlocks(blocks->previous_block, board);
    
    // Undo the shift
    // TODO : Regler ce probleme :)))
    if(!possible)
    {
        *blocks->block = tmp_ptr;
        *((*blocks->block)) = tmp_f;
        *((*blocks->block)+1) = tmp_s;
    }

    return possible;
}

const int leftShiftBlockListBlocks(struct BlockNode_s* const blocks, enum Block_e* const board)
{
    if(!blocks)
        return 1;
    
    const unsigned int block_col = getBlockCol(*blocks->block, board);
    if((block_col == 0) || *((*blocks->block)-1) != NONE)
        return 0;
    
    // Shift to the right
    const enum Block_e tmp_f = *((*blocks->block));
    const enum Block_e tmp_s = *((*blocks->block)-1);
    
    *((*blocks->block)-1) = *((*blocks->block));
    *((*blocks->block)) = NONE;

    //Update block node
    const enum Block_e* tmp_ptr = *blocks->block;
    *blocks->block = (*blocks->block)-1;

    const int possible = leftShiftBlockListBlocks(blocks->next_block, board);
    
    // Undo the shift
    if(!possible)
    {
        *blocks->block = tmp_ptr;
        *((*blocks->block)) = tmp_f;
        *((*blocks->block)-1) = tmp_s;
        //*blocks->block = (*blocks->block)-1;
    }

    return possible;
}

const int downShiftBlockListBlocks(struct BlockNode_s* const blocks, enum Block_e* const board)
{
    if(!blocks)
        return 1;
    const unsigned int block_row = getBlockRow(*blocks->block, board);
    if((block_row == gNumberRows - 1) || *((*blocks->block)+gNumberCols) != NONE)
        return 0;
    
    // Shift to the right
    const enum Block_e tmp_f = *((*blocks->block));
    const enum Block_e tmp_s = *((*blocks->block)+gNumberCols);
    
    *((*blocks->block)+gNumberCols) = *((*blocks->block));
    *((*blocks->block)) = NONE;

    //Update block node
    enum Block_e* tmp_ptr = *blocks->block;
    *blocks->block = (*blocks->block)+gNumberCols;

    const int possible = downShiftBlockListBlocks(blocks->previous_block, board);
    
    // Undo the shift
    if(!possible)
    {
        *blocks->block = tmp_ptr;
        *((*blocks->block)) = tmp_f;
        *((*blocks->block)+gNumberCols) = tmp_s;
        //*blocks->block = (*blocks->block)+gNumberCols;
    }

    return possible;
}

enum Block_e** getBoundingBlocks(struct BlockList_s* block_list, enum Block_e* board)
{
    // Count the number of blocks in a bloxk list
    unsigned int block_in_list_count = 0;
    unsigned int block_count = 0;
    struct BlockNode_s* cursor = block_list->blocks;

    int pivot_row = getBlockRow(*block_list->pivot_block->block, board);
    int pivot_col = getBlockCol(*block_list->pivot_block->block, board);

    enum Block_e** bounding_blocks;
    if(block_list->type != I)
    {
        bounding_blocks = (enum Block_e**)malloc(sizeof(enum Block_e*) * 9);
        for (int idx = 0, row = pivot_row-1; row <= pivot_row+1; row++)
        {
            for (int col = pivot_col-1; col <= pivot_col+1; col++, idx++)
            {
                if((row < 0) || (row >= gNumberRows) || (col < 0) || (col >= gNumberCols))
                {
                    free(bounding_blocks);
                    return NULL;
                }  
                if(*(board+row*gNumberCols+col) != NONE)
                    block_count++;  
                bounding_blocks[idx] = board+row*gNumberCols+col;
            }
        }
    }

    else{
        bounding_blocks = (enum Block_e**)malloc(sizeof(enum Block_e*) * 16);
        for (int idx = 0, row = pivot_row-2; row <= pivot_row+1; row++)
        {
            for (int col = pivot_col-2; col <= pivot_col+1; col++, idx++)
            {
                if((row < 0) || (row >= gNumberRows) || (col < 0) || (col >= gNumberCols))
                {
                    free(bounding_blocks);
                    return NULL;
                }  
                if(*(board+row*gNumberCols+col) != NONE)
                    block_count++;  
                bounding_blocks[idx] = board+row*gNumberCols+col;
            }
        }
    }
    
    return bounding_blocks;
} 

int getBlockIndexInBounding(enum Block_e** bounding_blocks, enum Block_e* block)
{
    for (size_t i = 0; i < 9; i++)
    {
        if(block == *(bounding_blocks+i))
            return i;
    }
    return -1;
}  

int getBlockIndexInBoudingI(enum Block_e** bounding_blocks, enum Block_e* block)
{
    for (size_t i = 0; i < 16; i++)
    {
        if(block == *(bounding_blocks+i))
            return i;
    }
    return -1;
}

int qSortByAdress(const void* a, const void* b)
{
    return (*(enum Block_e**)a) - (*(enum Block_e**)b);
}

int rotateBlockList(struct BlockList_s* block_list, enum Block_e* board)
{
    if(!block_list->pivot_block)
        return 0;
        // Keep the pointer to the pivot block on the board
    enum Block_e* pivot = *block_list->pivot_block->block;
    enum Block_e* last = *block_list->last_block->block;
    int is_last = 0;
    // Get bounding block around the shape 
    enum Block_e** bounding_blocks = getBoundingBlocks(block_list, board);
     
    if(!bounding_blocks)
        return 0;

    enum Block_e* new_block[16];

    struct BlockNode_s* cursor = block_list->blocks;
    enum Block_e block = block_list->block;
    int last_idx = -1;
    int idx = 0;
    while(cursor)
    {
        if(block_list->type != I)
        {
            switch (getBlockIndexInBounding(bounding_blocks, *cursor->block))
            {
            case 0:
                new_block[idx++] = bounding_blocks[2];
                break;
            case 1:
                new_block[idx++] = bounding_blocks[5];     
                break;
            case 2:
                new_block[idx++] = bounding_blocks[8];
                break;
            case 3:
                new_block[idx++] = bounding_blocks[1];
                break;
            case 4:
                new_block[idx++] = bounding_blocks[4];
                break;
            case 5:
                new_block[idx++] = bounding_blocks[7];
                break;
            case 6:
                new_block[idx++] = bounding_blocks[0];
                break;
            case 7:
                new_block[idx++] = bounding_blocks[3];
                break;
            case 8:
                new_block[idx++] = bounding_blocks[6];
                break;
            default:
                break;
            }
        }
        else
        {
            switch (getBlockIndexInBoudingI(bounding_blocks, *cursor->block))
            {
            case 0:
                new_block[idx++] = bounding_blocks[3];
                break;
            case 1:
                new_block[idx++] = bounding_blocks[7];     
                break;
            case 2:
                new_block[idx++] = bounding_blocks[8];
                break;
            case 3:
                new_block[idx++] = bounding_blocks[15];
                break;
            case 4:
                new_block[idx++] = bounding_blocks[1];
                break;
            case 5:
                new_block[idx++] = bounding_blocks[6];
                break;
            case 6:
                new_block[idx++] = bounding_blocks[9];
                break;
            case 7:
                new_block[idx++] = bounding_blocks[14];
                break;
            case 8:
                new_block[idx++] = bounding_blocks[2];
                break;
            case 9:
                new_block[idx++] = bounding_blocks[6];
                break;
            case 10:
                new_block[idx++] = bounding_blocks[10];
                break;
            case 11:
                new_block[idx++] = bounding_blocks[14];
                break;
            case 12:
                new_block[idx++] = bounding_blocks[0];
                break;
            case 13:
                new_block[idx++] = bounding_blocks[4];
                break;
            case 14:
                new_block[idx++] = bounding_blocks[11];
                break;
            case 15:
                new_block[idx++] = bounding_blocks[12];
                break;
            }
        }
       
        cursor = cursor->next_block;
    }

    // Quick sort new block based on the row and col of the block
    qsort(new_block, idx, sizeof(enum Block_e*), qSortByAdress);

    cursor = block_list->blocks;
    while(cursor)
    {
        **cursor->block = NONE;
        cursor = cursor->next_block;
    }
    int possible = 1;
    for (size_t i = 0; i < idx; i++)
    {
        if(*new_block[i] != NONE)
        {
            cursor = block_list->blocks;
            while(cursor)
            {
                **cursor->block = block;
                cursor = cursor->next_block;
            }
            free(bounding_blocks);
            return 0;
        }
    }
    cursor = block_list->blocks;
    for (size_t i = 0; i < idx; i++)
    {
        //free(cursor->block);
        //cursor->block = malloc(sizeof(enum Block_e**));
        *cursor->block = new_block[i];
        **cursor->block = block;
        if(i == idx - 1)
            block_list->last_block = cursor;
        if((block_list->type != I) && new_block[i] == bounding_blocks[4])
            block_list->pivot_block = cursor;
        else if((block_list->type == I) && new_block[i] == bounding_blocks[10]) 
            block_list->pivot_block = cursor;
        
        cursor = cursor->next_block;
    }

    free(bounding_blocks);
    return 1;

    const unsigned int N = 3;
    enum Block_e tmp;
    for (int x = 0; x < N/2; x++)
    {
        for (int y = x; y < N - x - 1; y++)
        {
            tmp = **(bounding_blocks+x*N+y);
            **(bounding_blocks+x*N+y) = **(bounding_blocks+y*N+N-1-x);
            **(bounding_blocks+y*N+N-1-x) = **(bounding_blocks+N*(N-1-x)+N-1-y);
            **(bounding_blocks+(N-1-x)*N+N-1-y) = **(bounding_blocks+N*(N-1-y)+x);
            **(bounding_blocks + N*(N-1-y)+x) = tmp;     
        }
        
    }
    cursor = block_list->blocks;
    int to_check[9] = {0};
    //int last = -1;
    while(cursor)
    {
        switch (getBlockIndexInBounding(bounding_blocks, *cursor->block))
        {
            case 2:
                to_check[idx++] = 0;
                if(0 > last)
                    last = 0;
                break;
            case 5:
                to_check[idx++] = 1;
                if(1 > last)
                    last = 1;
                break;
            case 8:
                to_check[idx++] = 2;
                if(2 > last)
                    last = 2;
                break;
            case 1:
                to_check[idx++] = 3;
                if(3 > last)
                    last = 3;
                break;
            case 4:
                to_check[idx++] = 4;
                if(4 > last)
                    last = 4;
                break;
            case 7:
                to_check[idx++] = 5;
                if(5 > last)
                    last = 5;
                break;
            case 0:
                to_check[idx++] = 6;
                if(6 > last)
                    last = 6;
                break;
            case 3:
                to_check[idx++] = 7;
                if(7 > last)
                    last = 7;
                break;
            case 6:
                to_check[idx++] = 8;
                if(8 > last)
                    last = 8;
                break;
        }
        cursor = cursor->next_block;
    }

    cursor = block_list->blocks;
    for (size_t i = 0; i < 9; i++)
    {
        // If corresponding to previous 
        if(**(bounding_blocks+i) != NONE)
        {
            // Check if in 
            for (size_t j = 0; j < 9; j++)
            {
                if(to_check[j] == i)
                {
                    free(cursor->block);
                    cursor->block = malloc(sizeof(enum Block_e**));
                    *cursor->block = *(bounding_blocks+i);
                     // If pivot block update
                    if(*cursor->block == pivot)
                        block_list->pivot_block = cursor;
                    if(to_check[j] == last)
                        block_list->last_block = cursor;
                    cursor = cursor->next_block;
                    break;
                }
            }
        }

    }
    
    //freeBlockList(block_list);
    free(bounding_blocks);
}

enum Block_e * createBoard()
{
    enum Block_e* board = calloc(gNumberRows*gNumberCols, sizeof(enum Block_e));
    return board;
}

int isRowFilled(enum Block_e* board, int row)
{
    for (size_t i = 0; i < gNumberCols; i++)
    {
        if(board[row*gNumberCols+i] == NONE)
            return 0;
    }
    return 1;
}

void clearFilledRows(enum Block_e* board)
{
    for (size_t i = 0; i < gNumberRows; i++)
    {
        if(isRowFilled(board, i))
        {
            for (size_t j = 0; j < gNumberCols; j++)
            {
                board[i*gNumberCols+j] = NONE;
            }
        }
    }
}

int isRowContainBlocks(enum Block_e* board, int row)
{
    for (size_t i = 0; i < gNumberCols; i++)
    {
        if(board[row*gNumberCols+i] != NONE)
            return 1;
    }
    return 0;
}

int downShiftBlock(enum Block_e* board, enum Block_e* block)
{
    int row = getBlockRow(block, board);
    int col = getBlockCol(block, board);
    if((board[(row)*gNumberCols+col] != NONE) && (row < gNumberRows-1) && (board[(row+1)*gNumberCols+col] == NONE))
    {
        board[(row+1)*gNumberCols+col] = *block;
        board[row*gNumberCols+col] = NONE;
        return 1;
    }
    return 0;
}

void clearRow(enum Block_e* board, int row)
{
    for (size_t i = 0; i < gNumberCols; i++)
    {
        board[row*gNumberCols+i] = NONE;
    }
}

unsigned int getBlockRow(const enum Block_e* const restrict block, const enum Block_e* const restrict board)
{
    ptrdiff_t ptr_diff = block - board;
    return ptr_diff/gNumberCols;
}

unsigned int getBlockCol(const enum Block_e* const restrict block, const enum Block_e* const restrict board)
{
    ptrdiff_t ptr_diff = block - board;
    return ptr_diff%(gNumberCols);
}

int moveBlockDown(enum Block_e* const restrict block, enum Block_e* const restrict board)
{
    unsigned int block_row = getBlockRow(block, board);
    if((block_row < gNumberRows - 1) && (*(block+gNumberCols) == NONE))
    {
        *(block + gNumberCols) = *block;
        *block = NONE;
        return 1;
    }
    return 0;
}

void moveBlockRight(enum Block_e* const restrict block, enum Block_e* const restrict board)
{
    unsigned int block_col = getBlockCol(block, board);
    if(block_col < gNumberCols - 1)
    {
        *(block + 1) = *block;
        *block = NONE;
    }
}

void moveBlockLeft(enum Block_e* const restrict block, enum Block_e* const restrict board)
{
    unsigned int block_col = getBlockCol(block, board);
    if(block_col > 0)
    {
        *(block - 1) = *block;
        *block = NONE;
    }
}

void moveAllBlocksDown(enum Block_e* const board)
{
    for (int i = gNumberCols * (gNumberRows-1); i >= 0; i--)
    {
        if(*(board+i) != NONE)
            moveBlockDown(board+i, board);
    }
    
}

void addBlockToCol(enum Block_e* restrict block, enum Block_e* restrict board, int col)
{
    if(col <= gNumberCols)
        *(board+col) = *block;
}

void printBoard(const enum Block_e* const board)
{
    for (size_t i = 0; i < gNumberCols * gNumberRows; i++)
    {
        if(!(i%gNumberCols))
            printf("\n");
        if(*(board + i) != NONE)
        {
            printf("\033[0;31m");
            printf("#");
            printf("\033[0;3m");
        }
        else 
            printf("o");
    }
    printf("\n");
}

// Down shift a block until it touches the ground
void shiftAllBlocksToGround(enum Block_e* const board, int row)
{
    while(isRowContainBlocks(board, row))
    {
        for (size_t col = 0; col < gNumberCols; col++)
        {
            int i = 0;
            while(moveBlockDown(board+(row + i)*gNumberCols+col, board))
            {
                i++;
            }
        }  
        row--;      
    }   
}

void printBlockList(struct BlockList_s* block_list, enum Block_e* board)
{
    struct BlockNode_s* cursor = block_list->blocks;
    while(cursor)
    {
        cursor = cursor->next_block;
    }
}

void freeBlockList(struct BlockList_s* block_list)
{

    if(!block_list)
        return;
    struct BlockNode_s* cursor = block_list->blocks;
    struct BlockNode_s* next_block = NULL;
    // print all node addresses
    cursor = block_list->blocks;
    while(cursor)
    {
        next_block = cursor->next_block;
        // BUG ???
        if(cursor->block)
            free(cursor->block);
        if(cursor)
            free(cursor);
        cursor = next_block;
    }

    free(block_list);
}

unsigned int getScoreFromMove(unsigned int nb_destroyed_rows, unsigned int level)
{
    switch (nb_destroyed_rows)
    {
        case 1:
            return 40 * (level+1);
        case 2:
            return 100 * (level+1);
        case 3:
            return 300 * (level+1);
        case 4:
            return 1200 * (level+1);
    }
}

void updateLevel(unsigned int* level, unsigned int* lines)
{
    static int start_level = -1;
    static int previous_line = -1;

    if(start_level < 0)
        start_level = *level;
    if(previous_line < 0)
        previous_line = *lines;

    static int gap_clear = 0;

    if(!gap_clear && (*lines >= start_level * 10 + 10))
    {
        gap_clear = 1;
        *level += 1;
        previous_line = *lines;
    }
    else if(gap_clear && (*lines >= previous_line + 10))
    {
        *level += 1;
        previous_line = *lines;
    }
    return;
    if((*level == 0) && (*lines >= 10))
    {
        *level = 1;
    }
    else if((*level == 1) && (*lines >= 20))
    {
        *level = 2;
    }
    else if((*level == 2) && (*lines >= 30))
    {
        *level = 3;
    }
    else if((*level == 3) && (*lines >= 40))
    {
        *level = 4;
    }
    else if((*level == 4) && (*lines >= 50))
    {
        *level = 5;
    }
    else if((*level == 5) && (*lines >= 60))
    {
        *level = 6;
    }
    else if((*level == 6) && (*lines >= 70))
    {
        *level = 7;
    }
    else if((*level == 7) && (*lines >= 80))
    {
        *level = 8;
    }
    else if((*level == 8) && (*lines >= 90))
    {
        *level = 9;
    }
    else if((*level == 9) && (*lines >= 100))
    {
        *level = 10;
    }
    else if((*level == 10) && (*lines >= 100))
    {
        *level = 11;
        *lines = 0;
    }
    else if((*level == 11) && (*lines >= 100))
    {
        *level = 12;
        *lines = 0;
    }
    else if((*level == 12) && (*lines >= 100))
    {
        *level = 13;
        *lines = 0;
    }
    else if((*level == 13) && (*lines >= 100))
    {
        *level = 14;
        *lines = 0;
    }
    else if((*level == 14) && (*lines >= 100))
    {
        *level = 15;
        *lines = 0;
    }
    else if((*level == 15) && (*lines >= 100))
    {
        *level = 16;
        *lines = 0;
    }
    else if((*level == 16) && (*lines >= 110))
    {
        *level = 17;
        *lines = 0;
    }
    
}

void resetStatistics(unsigned int statistics[])
{
    for (size_t i = 0; i < 7; i++)
        statistics[i] = 0; 
}

void resetBoard(enum Block_e* board)
{
    for (size_t i = 0; i < gNumberRows; i++)
    {
        clearRow(board, i);
    }
    
}

struct GameState_s* createGameState()
{
    struct GameState_s* game_state = malloc(sizeof(struct GameState_s));
    game_state->statistics = malloc(sizeof(unsigned int) * 7);
    game_state->lines = 0;
    game_state->level = 0;
    game_state->score = 0;
    game_state->board = createBoard();
    game_state->current_tetromino = NULL;
    game_state->next_tetromino = NULL;

    return game_state; 
}

void updateGravity(unsigned int* restrict gravity, const unsigned int* restrict level)
{
    /*
    00 	48
    01 	43
    02 	38
    03 	33
    04 	28
    05 	23
    06 	18
    07 	13
    08 	8
    09 	6
    10–12 	5
    13–15 	4
    16–18 	3
    19–28 	2
    29+ 	1 
    */

    if(*level == 0)
        *gravity = 48;
    else if(*level == 1)
        *gravity = 43;
    else if(*level == 2)
        *gravity = 38;
    else if(*level == 3)
        *gravity = 33;
    else if(*level == 4)
        *gravity = 28;
    else if(*level == 5)
        *gravity = 23;
    else if(*level == 6)
        *gravity = 18;
    else if(*level == 7)
        *gravity = 13;
    else if(*level == 8)
        *gravity = 8;
    else if(*level == 9)
        *gravity = 6;
    else if(*level >= 10 && *level <= 12)
        *gravity = 5;
    else if(*level >= 13 && *level <= 15)
        *gravity = 4;
    else if(*level >= 16 && *level <= 18)
        *gravity = 3;
    else if(*level >= 19 && *level <= 28)
        *gravity = 2;
    else
        *gravity = 1;
}

enum Scene_e titleSceneLoop(SDL_Renderer* renderer, SDL_Texture* title_texture)
{

    SDL_RenderCopy(renderer, title_texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while(1)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            break;
        if(event.type == SDL_JOYBUTTONDOWN)
        {
            if(event.jbutton.button == 2)
                return MENU;
        }  
    }
}


int checkClearedLines(const enum Block_e* board, unsigned int lines_to_destroy[])
{
    int cleared_lines = 0;
    for (int i = gNumberRows-1; i >= 0; i--)
    {
        if(isRowFilled(board, i))
        {
            lines_to_destroy[cleared_lines] = i;
            cleared_lines++;
        }
    }
    return cleared_lines;
}

struct BlockList_s* getBlockListFromRow(const enum Block_e* board, const unsigned int row)
{
    struct BlockList_s* block_list = NULL;
    int reading_block = 0;
    for(size_t col = 0; col < gNumberCols; col++)
    {
        if(board[row * gNumberCols + col] != NONE)
        {
            if(!block_list)
            {
                block_list = createBlockList(0, board[row * gNumberCols + col]);
                reading_block = 1;
            }
            enum Block_e* block = board + row * gNumberCols + col;
            addNodeToBlockList(block_list, createBlockNode(&block));
        }
        else{
            if(reading_block)
                return block_list;
        }
    }

    return block_list;
}

void shiftDownSticky(enum Block_e* board, unsigned int row)
{
    while(isRowContainBlocks(board, row))
    {
        struct BlockList_s* block_list = getBlockListFromRow(board, row);
        struct BlockNode_s* cursor = block_list->blocks;
        while(block_list)
        {
            while(downShiftBlockListBlocks(block_list->last_block, board));
            freeBlockList(block_list);
            block_list = getBlockListFromRow(board, row);
        }
        row--;
    }
}

int isScoreTopScore(const unsigned int score)
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    size_t idx = 1;
    if(file)
    {
        char line[100];
        while(fgets(line, 100, file))
        {
            if(strlen(line) > 0)
            {
                line[strlen(line)-1] = '\0';
                const char* player_score_in_file = strtok(line, " ");
                player_score_in_file = strtok(NULL, " ");
                if(score > atoi(player_score_in_file))
                {
                    return idx;
                }
                idx++;
            }
        }
        return 0;
    }
}

void storeTopScore(const char* player_name, const unsigned int score)
{
    FILE* file = fopen("scores/top_score.txt", "r+");
    if(file)
    {
        // Read file line by line 
        char line[100];
        while(fgets(line, 100, file))
        {
            // If line is not empty
            if(strlen(line) > 0)
            {
                // Remove newline character
                line[strlen(line)-1] = '\0';
                // If line is not the player name
                const char* player_name_in_file = strtok(line, " ");
                if(!strcmp(player_name_in_file, player_name))
                {
                    // Write line to file
                    fprintf(file, "%s %d\n", player_name_in_file, score);
                    return;
                }
            }
        }
        
        fprintf(file, "%s %d\n", player_name, score);

        fclose(file);
    }
}

static int sceIoMove(const char *src, const char *dest) {
        int ret = 0;
        size_t i = 0;
        char strage[32] = {0};
        char *p1 = NULL, *p2 = NULL;
        p1 = strchr(src, ':');
        
        if (p1 == NULL)
            return -1;
            
        p2 = strchr(dest, ':');
        if (p2 == NULL)
            return -1;
            
        if ((p1 - src) != (p2 - dest))
            return -1;
            
        for (i = 0; (src + i) <= p1; i++) {
            if ((i+1) >= sizeof(strage))
                return -1;
                
            if (src[i] != dest[i])
                return -1;
            
            strage[i] = src[i];
        }
        
        strage[i] = '\0';

        u32 data[2] = {0};
        data[0] = (u32)(p1 + 1);
        data[1] = (u32)(p2 + 1);

        return 0;
    }

void changeTopScore(unsigned int score, unsigned int level, unsigned int place)
{
    SceUID* psp_file = sceIoOpen("scores/top_score.txt", PSP_O_WRONLY|PSP_O_CREAT, 0777);
    sceIoClose(psp_file);
    FILE* file = fopen("scores/top_score.txt", "r");
    SceUID* tmp_psp_file = sceIoOpen("scores/tmp.txt", PSP_O_WRONLY|PSP_O_CREAT, 0777);
    sceIoClose(tmp_psp_file);
    FILE* tmp_file = fopen("scores/tmp.txt", "w");

    char line[100];
    char new_line[100];
    size_t idx = 0;
    char read_char;
    if(file)
    {
        /*while(fgets(line, 100, file))
        {
            line[strlen(line)-1] = '\0';
            if(place == idx)
            {
                const char* player_score_in_file = strtok(line, " ");
                sprintf(new_line, "%s %d %d\n", player_score_in_file, score, level);
                fprintf(tmp_file, new_line);
            }
            else
                fprintf(tmp_file, line);

            idx++;
        }*/
    }
    fclose(file);
    fclose(tmp_file);
    if(sceIoRemove("ms0:PSP/GAMEscores/top_score.txt"))
        exit(1);
    //sceIoMove("scores/tmp.txt", "scores/top_score.txt");
    //sceIoRename("scores/tmp.txt", "scores/top_score.txt");
}

#define printf pspDebugScreenPrintf


int main(int argc, char* argv[]) {

    srand(time(NULL));

    enum Block_e* board = createBoard();
    int has_touch_down = 0;
    int spawn_block = 4 + 1 * gNumberCols;

	pspDebugScreenInit();

    SDL_Window* window;
    SDL_Renderer* renderer;
    initGraphics(&window, &renderer);
    initSounds();
    SDL_Surface* playground = loadPlayGround("gfx/playground.png");
    SDL_Surface* scored_playground = loadPlayGround("gfx/scored_playground.png");
    //SDL_Surface* character_surface = createCharacterSurfaceFromSprite(characters_sprite, '9');
    SDL_Texture* title_screen_texture = loadTitleSceneTexture(renderer);
    SDL_Texture* menu_texture = loadMenuSceneTexture(renderer);
    SDL_Surface* menu_a_surface = IMG_Load("gfx/menu_a.png");
    SDL_Texture* menu_a_texture = loadMenuASceneTexture(renderer);
    SDL_Texture* playground_texture = SDL_CreateTextureFromSurface(renderer, playground);
    SDL_Texture* scored_playground_texture = SDL_CreateTextureFromSurface(renderer, scored_playground);
    SDL_Texture** A_scene_textures = loadASceneTextures(renderer);
    SDL_Surface* master_a_surface = IMG_Load("gfx/master_a.png");
    SDL_Texture* master_a_texture = A_scene_textures[MASTER];



    SDL_FreeSurface(playground);
    SDL_FreeSurface(scored_playground);


    OSL_SOUND* music = loadMusic("musics/music_1.mp3");
    OSL_SOUND* music_2 = loadMusic("musics/music_2.mp3");
    OSL_SOUND* music_3 = loadMusic("musics/music_3.mp3");
    oslSetSoundLoop(music, -1);
    oslSetSoundLoop(music_2, -1);
    oslSetSoundLoop(music_3, -1);
    OSL_SOUND* music_playlist[3] = {music, music_2, music_3};

    OSL_SOUND* clear_sfx = loadSFX("sfx/clear_sfx.wav");
    OSL_SOUND* row_sfx = loadSFX("sfx/row_sfx.wav");
    OSL_SOUND* level_sfx = loadSFX("sfx/level_sfx.wav");
    OSL_SOUND* shift_sfx = loadSFX("sfx/shift_sfx.wav");
    OSL_SOUND* rotate_sfx = loadSFX("sfx/rotate_sfx.wav");
    OSL_SOUND* ground_sfx = loadSFX("sfx/ground_sfx.wav");

    g_select_sfx = loadSFX("sfx/select_sfx.wav");


    SDL_Event event;
    SDL_Joystick* joystick = SDL_JoystickOpen(0);

    long unsigned int current_time = 0;
    long unsigned int frame_start = 0;
    long unsigned int msec = 0;
    long unsigned int countedFrames = 0;


    int down_holded = 0;
    int left_holded = 0;
    int right_holded = 0;
    int down_holded_frames = 0;
    int left_holded_frames = 0;
    int right_holded_frames = 0;

    int game_type, music_type;
    struct GameState_s* game_state = createGameState();

    SDL_Rect block_rect = {177, 47, 9, 9};
    SDL_Rect lines_rect = {284, 19, 13, 9};
    SDL_Rect board_rect = {178, 48, 149, 194};
    unsigned int lines_to_destroy[gNumberRows];
    unsigned int statistics[7] = {0};
    game_state->statistics = statistics;
    unsigned int score = 0;
    unsigned int level = 0;
    unsigned int lines = 0;
    int nb_lines_to_destroy = 0;

    enum Scene_e scene = MASTER_A;
    game_state->score = 2500;
    unsigned int top_score_place = 0;
MAIN_LOOP:
    while(scene != GAME_A)
    {
        if(scene == TITLE)
            scene = titleSceneLoop(renderer, title_screen_texture);
        else if(scene == MENU)
           {scene = menuSceneLoop(renderer, menu_texture, &game_type, music_playlist);
           scene = MENU_A;
           }
        else if(scene == MENU_A)
            scene = menuASceneLoop(renderer, &menu_a_texture, menu_a_surface, &game_state->level);
        else if(scene == MASTER_A)
            scene = masterASceneLoop(renderer, master_a_surface, A_scene_textures, game_state);
        
        SDL_FlushEvent(SDL_JOYBUTTONDOWN);
        SDL_PumpEvents();
    }

struct BlockList_s* tetrimino = generateRandomTetrimino(board+spawn_block, board);
    struct BlockList_s* next_tetromino = generateRandomTetrimino(board+spawn_block, board);
    loadTetrominoToBoard(tetrimino, board);
     game_state->board = board;
    game_state->current_tetromino = tetrimino;
    game_state->next_tetromino = next_tetromino;
    unsigned int gravity = 48;

    updateGravity(&gravity, &game_state->level);
    //MenuLoop(renderer, menu_texture);
    int soft_drop = 0;
    do
    {          
        if(!game_state->current_tetromino)
            break;
         //Calculate and correct fps
        frame_start = SDL_GetTicks();
        float avgFPS = countedFrames / ( SDL_GetTicks() / 1000.f );
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }
        // Make the loop each second with sdl ticks
        if(soft_drop)
            gravity = 2;
        else
            updateGravity(&gravity, &game_state->level);
        if(!(countedFrames%gravity) && (countedFrames != 0))
        {    current_time = SDL_GetTicks();
            has_touch_down = !downShiftBlockListBlocks(game_state->current_tetromino->last_block, game_state->board);
        }

        if(has_touch_down)
        {
            int row = gNumberRows-1;
            int row_filled = 0;
            nb_lines_to_destroy = checkClearedLines(game_state->board, lines_to_destroy);
            if(nb_lines_to_destroy)
            {
                SDL_Texture* playground_to_show = playground_texture;
                if(nb_lines_to_destroy >= 4)
                    oslPlaySound(clear_sfx, 1);
                else
                    oslPlaySound(row_sfx, 1);
                SDL_Delay(5);

                for(int i = 4, j = 5; i >= 0; i--, j++)
                {
                for (size_t k = 0; k < nb_lines_to_destroy; k++)
                {
                        game_state->board[lines_to_destroy[k]*gNumberCols + j]= NONE;
                        game_state->board[lines_to_destroy[k]*gNumberCols + i]= NONE;
                }
                if(nb_lines_to_destroy >= 4)
                {
                    if(i%2)
                        playground_to_show = scored_playground_texture;
                    else
                        playground_to_show = playground_texture;
                }
                renderAScene(renderer, A_scene_textures, game_state);
                SDL_Delay(100);
                }

                // Moves all board blocks down
                for (size_t k = 0; k < nb_lines_to_destroy; k++)
                {
                    shiftDownSticky(game_state->board, lines_to_destroy[k]-1);
                }
                game_state->score += getScoreFromMove(nb_lines_to_destroy, game_state->level);
                game_state->lines += nb_lines_to_destroy;
                nb_lines_to_destroy = 0;
                unsigned int previous_level = game_state->level;
                updateLevel(&game_state->level, &game_state->lines);
                if(previous_level != game_state->level)
                {
                    oslPlaySound(level_sfx, 1);
                }
                //freeBlockList(tetrimino);
                //free(board);
                //exit(1);
            }
            game_state->statistics[game_state->current_tetromino->type]++;
            freeBlockList(game_state->current_tetromino);
            game_state->current_tetromino = game_state->next_tetromino; 
            game_state->next_tetromino = generateRandomTetrimino(game_state->board+spawn_block, game_state->board);
            loadTetrominoToBoard(game_state->current_tetromino, game_state->board);

            has_touch_down = 0;
        }

        int quit = 0;
        while(SDL_PollEvent(&event)){
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_JOYBUTTONDOWN:
                        switch(event.jbutton.button)
                        {
                            case SDLK_ESCAPE:
                                quit = 1;
                                break;
                            case 7:
                                oslPlaySound(shift_sfx, 1);
                                leftShiftBlockListBlocks(game_state->current_tetromino->blocks, board);
                                break;
                            case 9:
                                oslPlaySound(shift_sfx, 1);
                                rightShiftBlockListBlocks(game_state->current_tetromino->last_block, board);
                                break;
                            case 6:

                                if(!downShiftBlockListBlocks(game_state->current_tetromino->last_block, board))
                                {
                                    current_time = 0;                                    
                                }   
                                break;
                            case 2:
                                    oslStopSound(rotate_sfx);
                                if(rotateBlockList(game_state->current_tetromino, board))
                                    oslPlaySound(rotate_sfx, 1);
                                break;
                        }
                case SDL_JOYBUTTONUP:   
                    break;
            }
        }

        if(SDL_JoystickGetButton(joystick, 7))
        {
            left_holded_frames++;
            if(left_holded_frames >= 16)
            {
                leftShiftBlockListBlocks(game_state->current_tetromino->blocks, board);
                oslPlaySound(shift_sfx, 1);
                left_holded_frames = 10;
            }
        }   
        else
            left_holded_frames = 0; 
        if(SDL_JoystickGetButton(joystick, 9))
        {
            right_holded_frames++;
            if(right_holded_frames >= 16)
            {
                oslPlaySound(shift_sfx, 1);
                rightShiftBlockListBlocks(game_state->current_tetromino->last_block, board);
                right_holded_frames = 10;
            }
        }   
        else
            right_holded_frames = 0;
        if(SDL_JoystickGetButton(joystick, 6))
        {
            down_holded_frames++;
            soft_drop = 1;
            if(down_holded_frames >= 16)
            {
                if(!downShiftBlockListBlocks(game_state->current_tetromino->last_block, board))
                {
                    oslPlaySound(ground_sfx, 1);
                    has_touch_down = 1;
                }
                down_holded_frames = 10;
            }
        }   
        else{
            down_holded_frames = 0;
            soft_drop = 0;
        }
        renderAScene(renderer, A_scene_textures, game_state);
        ++countedFrames;
        //If frame finished early
        int frameTicks = SDL_GetTicks() - frame_start;
        if( frameTicks < 1000/60 )
        {
                    //Wait remaining time
            SDL_Delay( 1000/60 - frameTicks );
        }
        if(countedFrames >= 60)
            countedFrames = 0;

    }while(game_state->current_tetromino);
    
    for (size_t i = 0; i < gNumberRows; i++)
    {
        for (size_t j = 0; j < gNumberCols; j++)
        {
            game_state->board[i*gNumberCols + j] = FOUR;
            renderAScene(renderer, A_scene_textures, game_state);
        }
        
    }

    for (size_t i = 0; i < gNumberRows; i++)
    {
        for (size_t j = 0; j < gNumberCols; j++)
        {
            game_state->board[i*gNumberCols + j] = NONE;
        }
    }

    top_score_place = isScoreTopScore(game_state->score);
    if(top_score_place)
        scene = MASTER_A;
    else 
        scene = MENU_A;
    goto MAIN_LOOP;
}
