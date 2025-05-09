#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

// sets up the game
void init_game(struct gamestate* g) {
    srand(time(NULL));
    
    g->player_x = GRID_SIZE / 2;
    g->player_y = GRID_SIZE / 2;
    
    g->fuel_level = 100;
    g->points = 0;
    
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            g->grid[i][j] = EMPTY;
        }
    }
    
    place_player(g);
    place_junk(g);
}

// shows the grid on screen
void print_grid(struct gamestate* g) {
    printf("\n");  // new line
    
    // top line
    for(int i = 0; i < GRID_SIZE + 2; i++) {
        printf("-");
    }
    printf("\n");
    
    // the actual grid
    for(int i = 0; i < GRID_SIZE; i++) {
        printf("|");  // left side
        for(int j = 0; j < GRID_SIZE; j++) {
            printf("%c", g->grid[i][j]);
        }
        printf("|\n");  // right side
    }
    
    // bottom line
    for(int i = 0; i < GRID_SIZE + 2; i++) {
        printf("-");
    }
    printf("\n");
}

// puts the player on the grid
void place_player(struct gamestate* g) {
    g->grid[g->player_y][g->player_x] = PLAYER;
}

// adds random junk to the grid
void place_junk(struct gamestate* g) {
    int num_junk = 10;  // how much junk to add
    
    for(int i = 0; i < num_junk; i++) {
        int x, y;
        do {
            x = rand() % GRID_SIZE;
            y = rand() % GRID_SIZE;
        } while(g->grid[y][x] != EMPTY);  // keep trying till we find empty spot
        
        g->grid[y][x] = JUNK;
    }
}

int main() {
    struct gamestate g;
    
    printf("Welcome to SpaceXplorer!\n");
    printf("Starting game...\n");
    
    init_game(&g);
    print_grid(&g);
    
    printf("\nGame ready! P is player, J is junk\n");
    printf("Fuel: %d\n", g.fuel_level);
    
    return 0;
} 