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
    g->num_asteroids = MAX_ASTEROIDS;
    
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            g->grid[i][j] = EMPTY;
        }
    }
    
    place_player(g);
    place_junk(g);
    place_asteroids(g);
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

// adds junk to the grid
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

// place asteroids on left side
void place_asteroids(struct gamestate* g) {
    for(int i = 0; i < g->num_asteroids; i++) {
        g->asteroid_x[i] = 0;
        g->asteroid_y[i] = rand() % GRID_SIZE;
        g->grid[g->asteroid_y[i]][g->asteroid_x[i]] = ASTEROID;
    }
}

// move all asteroids
void move_asteroids(struct gamestate* g) {
    for(int i = 0; i < g->num_asteroids; i++) {
        // clear old position
        g->grid[g->asteroid_y[i]][g->asteroid_x[i]] = EMPTY;
        
        // move right
        g->asteroid_x[i]++;
        if(g->asteroid_x[i] >= GRID_SIZE) {
            g->asteroid_x[i] = 0;
            g->asteroid_y[i] = rand() % GRID_SIZE;
        }
        
        // place asteroid in new position
        g->grid[g->asteroid_y[i]][g->asteroid_x[i]] = ASTEROID;
    }
}

// check if player hit asteroid
int check_collision(struct gamestate* g) {
    for(int i = 0; i < g->num_asteroids; i++) {
        if(g->player_x == g->asteroid_x[i] && g->player_y == g->asteroid_y[i]) {
            return 1;  // collision!
        }
    }
    return 0;  // no collision
}

// move player
void move_player(struct gamestate* g, char direction) {
    // clear old position
    g->grid[g->player_y][g->player_x] = EMPTY;
    
    // update position based on direction
    switch(direction) {
        case 'w':  // up
            if(g->player_y > 0) g->player_y--;
            break;
        case 's':  // down
            if(g->player_y < GRID_SIZE-1) g->player_y++;
            break;
        case 'a':  // left
            if(g->player_x > 0) g->player_x--;
            break;
        case 'd':  // right
            if(g->player_x < GRID_SIZE-1) g->player_x++;
            break;
    }
    
    // place player in new position
    g->grid[g->player_y][g->player_x] = PLAYER;
}

int main() {
    struct gamestate g;
    char move;
    
    printf("Welcome to SpaceXplorer!\n");
    printf("Starting game...\n");
    
    init_game(&g);
    
    while(1) {
        print_grid(&g);
        printf("\nGame ready! P is player, J is junk, A is asteroid\n");
        printf("Fuel: %d\n", g.fuel_level);
        printf("Enter move (w=up, s=down, a=left, d=right, q=quit): ");
        
        scanf(" %c", &move);
        
        if(move == 'q') break;
        
        // move player
        move_player(&g, move);
        
        // check for collision
        if(check_collision(&g)) {
            printf("\nGame Over! You hit an asteroid!\n");
            break;
        }
        
        // move asteroids
        move_asteroids(&g);
        
        // check for collision after asteroid move
        if(check_collision(&g)) {
            printf("\nGame Over! An asteroid hit you!\n");
            break;
        }
    }
    
    return 0;
} 