#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "game.h"

void save_score(char* name, int score, int fuel);
void show_leaderboard(char* name, int score, int fuel);
void clear_input();
// sets up the game
void init_game(struct gamestate* g) {
    srand(time(NULL));
    g->difficulty = 2;//3 is medium: 2 is easy 4 is hard
    g->player_x = GRID_SIZE / 2;
    g->player_y = GRID_SIZE / 2;
    
    g->fuel_level = 200;
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
    printf("Collect 10 points by picking up Junk to win\n");
    printf("Before the feul runs out!!!\n");
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



// check if player hit asteroid
int check_collision(struct gamestate* g) {
    if(g->collision_flag == 1){
        return 1;
    }
    for(int i = 0; i < g->num_asteroids; i++) {
        if(g->player_x == g->asteroid_x[i] && g->player_y == g->asteroid_y[i]) {
            // show X at collision
            g->grid[g->player_y][g->player_x] = 'X';
            return 1;  // collision!
        }
    }
    return 0;  // no collision
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
        if(g->grid[new_y][new_x] == ASTEROID){
            g->collision_flag = 1;
        }
        if(g->grid[new_y][new_x] == JUNK){
            g->collision_flag = 0;
            g->points +=1;
        }


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
    // probablity check for placing junk and asteriod
    int place_junk=0;
    int place_aster=0;
    if(rand() % g->difficulty == 0){
        place_junk = 1;
    }
    if(rand() % g->difficulty != 0){
        place_aster = 1;
    }
    for(int j = 0; j < GRID_SIZE; j++) {
        if(j == asteroid_pos && place_aster == 1) {
            g->grid[0][j] = ASTEROID;
        } else if(j == junk_pos && place_junk == 1) {
            g->grid[0][j] = JUNK;
        } else {
            g->grid[0][j] = EMPTY;
        }
    }
    // check if new player position has asteroid or junk
    if (g->grid[g->player_y][g->player_x] == ASTEROID) {
        g->grid[g->player_y][g->player_x] = 'X';
        g->collision_flag = 1; // set a flag for collision
    } else if (g->grid[g->player_y][g->player_x] == JUNK) {
        g->collision_flag = 0;
        g->grid[g->player_y][g->player_x] = PLAYER;
        g->points +=1;
    } else {
        g->collision_flag = 0;
        g->grid[g->player_y][g->player_x] = PLAYER;
    }
    g->fuel_level -= 1;

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
    char name[20];
    
    printf("Welcome to SpaceXplorer!\n");
    printf("Starting game...\n");

    printf("Enter your difficulty (2 is easy, 3 is medium, 4 is hard): ");
    scanf("%d", &g.difficulty);
    if(g.difficulty < 2 || g.difficulty > 4){
        printf("Invalid difficulty. Defaulting to medium.\n");
        g.difficulty = 3;
    }

    sleep_ms(1000);
    
    init_game(&g);
    
    while(!game_over) {
        print_grid(&g);
        
        move = get_input();
        clear_input();
        if(move == 'q') break;
        
        if(move) {
            move_player(&g, move);
        }
        
        if(check_collision(&g)) {
            print_grid(&g); // show X
            printf("\nGame Over! You hit an asteroid!\n");
            game_over = 1;
            break;
        }
        
        if(g.points >= 10){
            printf("You win! You collected 10 points!\n");
            printf("Enter your name (no spaces, max 19 chars): ");
            scanf("%19s", name);
            save_score(name, 10, g.fuel_level);
            show_leaderboard(name, 10, g.fuel_level);
            game_over = 1;
            break;
        }
        if(check_collision(&g)) {
            print_grid(&g); // show X
            printf("\nGame Over! An asteroid hit you!\n");
            game_over = 1;
            break;
        }
        if(g.fuel_level <= 0){
            print_grid(&g); 
            printf("\nGame Over! You ran out of fuel!\n");
            game_over = 1;
            break;
        }
        
        shift_grid_down(&g);
        sleep_ms(500);
    }
    
    return 0;
}

// leaderboard stuff
void save_score(char* name, int score, int fuel) {
    FILE *f = fopen("leaderboard.txt", "a");
    if(f) {
        fprintf(f, "%s %d %d\n", name, score, fuel);
        fclose(f);
    }
}

void show_leaderboard(char* name, int score, int fuel) {
    char n[100][20];
    int s[100], fu[100];
    int nplayers = 0;
    FILE *f = fopen("leaderboard.txt", "r");
    if(f) {
        while(fscanf(f, "%19s %d %d", n[nplayers], &s[nplayers], &fu[nplayers]) == 3 && nplayers < 100) nplayers++;
        fclose(f);
    }
    // sort by fuel desc, then score desc
    for(int i=0;i<nplayers-1;i++){
        for(int j=i+1;j<nplayers;j++){
            if(fu[j]>fu[i] || (fu[j]==fu[i] && s[j]>s[i])){
                int t=s[i];s[i]=s[j];s[j]=t;
                t=fu[i];fu[i]=fu[j];fu[j]=t;
                char tmp[20];
                strcpy(tmp, n[i]);
                strcpy(n[i], n[j]);
                strcpy(n[j], tmp);
            }
        }
    }
    int rank = 1;
    for(int i=0;i<nplayers;i++){
        if(strcmp(n[i],name)==0 && s[i]==score && fu[i]==fuel){
            rank = i+1;
            break;
        }
    }
    printf("Your rank: %d out of %d\n", rank, nplayers);
    printf("Top 5:\n");
    for(int i=0;i<5 && i<nplayers;i++){
        printf("%d. %s Score: %d Fuel: %d\n", i+1, n[i], s[i], fu[i]);
    }
} 

void clear_input() {
    int c;
    //while ((c = getchar()) != '\n' && c != EOF) {}
}