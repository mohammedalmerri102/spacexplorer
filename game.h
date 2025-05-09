#ifndef GAME_H
#define GAME_H

// grid size
#define GRID_SIZE 18

// Elemtns  in the grid
#define EMPTY ' '
#define PLAYER 'P'
#define JUNK 'J'
#define ASTEROID 'A'

// max number of asteroids
#define MAX_ASTEROIDS 3

// keeps track of everything in the game
struct gamestate {
    char grid[GRID_SIZE][GRID_SIZE];  // the board
    int player_x;                      // where player is
    int player_y;                      // player y pos
    int fuel_level;                    // fuel level
    int points;                        // score
    int asteroid_x[MAX_ASTEROIDS];    // array of asteroid positions
    int asteroid_y[MAX_ASTEROIDS];
    int num_asteroids;                // current number of asteroids
};

// functions we need
void init_game(struct gamestate* g);
void print_grid(struct gamestate* g);
void place_player(struct gamestate* g);
void place_junk(struct gamestate* g);
void place_asteroids(struct gamestate* g);
void move_asteroids(struct gamestate* g);
int check_collision(struct gamestate* g);

#endif 