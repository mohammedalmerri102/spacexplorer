#ifndef GAME_H
#define GAME_H

// grid size needs to be at least 18x18
#define GRID_SIZE 18

// stuff that can be in the grid
#define EMPTY ' '
#define PLAYER 'P'
#define JUNK 'J'
#define ASTEROID 'A'

// keeps track of everything in the game
struct gamestate {
    char grid[GRID_SIZE][GRID_SIZE];  // the board
    int player_x;                      // where player is
    int player_y;                      // player y pos
    int fuel_level;                    // how much fuel left
    int points;                        // score
};

// functions we need
void init_game(struct gamestate* g);
void print_grid(struct gamestate* g);
void place_player(struct gamestate* g);
void place_junk(struct gamestate* g);

#endif 