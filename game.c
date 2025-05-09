#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
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

// show the grid
void print_grid(struct gamestate* g) {
    system("cls");  // clear screen
    
    // top border
    for(int i = 0; i < GRID_SIZE + 2; i++) {
        printf("-");
    }
    printf("\n");
    
    // grid stuff
    for(int i = 0; i < GRID_SIZE; i++) {
        printf("|");
        for(int j = 0; j < GRID_SIZE; j++) {
            printf("%c", g->grid[i][j]);
        }
        printf("|\n");
    }
    
    // bottom border
    for(int i = 0; i < GRID_SIZE + 2; i++) {
        printf("-");
    }
    printf("\n");
    
    printf("Score: %d  Fuel: %d\n", g->points, g->fuel_level);
    printf("WASD to move, Q to quit\n");
}

// puts player on grid
void place_player(struct gamestate* g) {
    g->grid[g->player_y][g->player_x] = PLAYER;
}

// add some junk
void place_junk(struct gamestate* g) {
    int num_junk = 10;  // how much junk to add
    
    for(int i = 0; i < num_junk; i++) {
        int x, y;
        do {
            x = rand() % GRID_SIZE;
            y = rand() % GRID_SIZE;
        } while(g->grid[y][x] != EMPTY);  // find empty spot
        
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
        g->grid[g->asteroid_y[i]][g->asteroid_x[i]] = EMPTY;  // clear old spot
        
        g->asteroid_x[i]++;  // move right
        if(g->asteroid_x[i] >= GRID_SIZE) {
            g->asteroid_x[i] = 0;
            g->asteroid_y[i] = rand() % GRID_SIZE;
        }
        
        g->grid[g->asteroid_y[i]][g->asteroid_x[i]] = ASTEROID;  // put in new spot
    }
}

// check if player hit asteroid
int check_collision(struct gamestate* g) {
    for(int i = 0; i < g->num_asteroids; i++) {
        if(g->player_x == g->asteroid_x[i] && g->player_y == g->asteroid_y[i]) {
            return 1;  // hit!
        }
    }
    return 0;  // no hit
}

// move player
void move_player(struct gamestate* g, char direction) {
    int new_x = g->player_x;
    int new_y = g->player_y;
    
    // calculate new position
    switch(direction) {
        case 'w':  // up
            if(g->player_y > 0) new_y--;
            break;
        case 's':  // down
            if(g->player_y < GRID_SIZE-1) new_y++;
            break;
        case 'a':  // left
            if(g->player_x > 0) new_x--;
            break;
        case 'd':  // right
            if(g->player_x < GRID_SIZE-1) new_x++;
            break;
    }
    
    // only move if position changed
    if(new_x != g->player_x || new_y != g->player_y) {
        // clear old position
        g->grid[g->player_y][g->player_x] = EMPTY;
        
        // update position
        g->player_x = new_x;
        g->player_y = new_y;
        
        // set new position
        g->grid[g->player_y][g->player_x] = PLAYER;
    }
}

// new stuff for moving grid
void shift_grid_down(struct gamestate* g) {
    // clear old player position before shifting
    g->grid[g->player_y][g->player_x] = EMPTY;
    // move everything down
    for(int i = GRID_SIZE-1; i > 0; i--) {
        for(int j = 0; j < GRID_SIZE; j++) {
            g->grid[i][j] = g->grid[i-1][j];
        }
    }
    // add new row at top with max 1 asteroid and max 1 junk
    int asteroid_pos = rand() % GRID_SIZE;
    int junk_pos = rand() % GRID_SIZE;
    while (junk_pos == asteroid_pos) {
        junk_pos = rand() % GRID_SIZE; // make sure they're not the same
    }
    for(int j = 0; j < GRID_SIZE; j++) {
        if(j == asteroid_pos) {
            g->grid[0][j] = ASTEROID;
        } else if(j == junk_pos) {
            g->grid[0][j] = JUNK;
        } else {
            g->grid[0][j] = EMPTY;
        }
    }
    // move player up one row if not at top
    if (g->player_y > 0) {
        g->player_y--;
    } else {
        g->player_y = 0; // clamp at top
    }
    // make sure player is visible at new position
    g->grid[g->player_y][g->player_x] = PLAYER;
}

// get key without waiting
char get_input(void) {
    if(_kbhit()) {
        return _getch();
    }
    return 0;
}

// wait a bit
void sleep_ms(int ms) {
    Sleep(ms);
}

int main() {
    struct gamestate g;
    char move;
    int game_over = 0;
    
    printf("Welcome to SpaceXplorer!\n");
    printf("Starting game...\n");
    sleep_ms(1000);
    
    init_game(&g);
    
    while(!game_over) {
        print_grid(&g);
        
        move = get_input();
        if(move == 'q') break;
        
        if(move) {
            move_player(&g, move);
        }
        
        if(check_collision(&g)) {
            printf("\nGame Over! You hit an asteroid!\n");
            game_over = 1;
            break;
        }
        
        move_asteroids(&g);
        
        if(check_collision(&g)) {
            printf("\nGame Over! An asteroid hit you!\n");
            game_over = 1;
            break;
        }
        
        shift_grid_down(&g);
        sleep_ms(1000);
    }
    
    return 0;
} 