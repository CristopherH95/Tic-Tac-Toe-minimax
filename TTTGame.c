#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TTTGame.h"

//board_init
//parameters:   board is a matrix representing the playable board
//returns:      none
//this function initializes the board to null values
void board_init(int board[X][Y]) {
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            board[i][j] = 0;
        }
    }
}

//print_board
//parameters:   board is a matrix representing the playable board
//returns:      none
//this function prints the current board state
void print_board(const int board[X][Y]) {
    printf("     1   2   3  \n");
    for (int i = 0; i < Y; i++) {
        printf("%d  | %d | %d | %d |\n", (i+1), board[i][0], board[i][1], board[i][2]);
    }
}

//update_board
//parameters:   board is a matrix representing the playable board, player_x is the x-coordinate
//              of the desired player move, player_y is the y-coordinate of the desired player move
//returns:      int -1 for invalid move 0 for valid move
//this function takes the desired player move and validates it before placing it on the board
int update_board(int board[X][Y],int player_x, int player_y) {
    if (board[player_x][player_y] != 0 || (player_x < 0 || player_x > X)
        || (player_y < 0 || player_y > Y)) {
        fprintf(stderr, "Invalid move!\n");
        return -1;
    }
    board[player_x][player_y] = 1;
    fprintf(stderr, "move placed\n");
    return 0;
}

//get_max_score
//parameters:   score_a and score_b are two scores to compare
//returns:      whichever score is greater than the other
//this function will determine a max score between 2 scores
int get_max_score(int score_a, int score_b) {
    if (score_a > score_b) {
        return score_a;
    }
    else {
        return score_b;
    }
}

//get_min_score
//parameters:   score_a and score_b are two scores to compare
//returns:      whichever score is smaller than the other
//this function will determine a min score between 2 scores
int get_min_score(int score_a, int score_b) {
    if (score_a < score_b) {
        return score_a;
    }
    else {
        return score_b;
    }
}

//find_empty_space
//parameters:   board is a matrix representing the playable board, x_coord and y_coord are where the empty space found
//              will be stored
//returns:      none
//this function will find the first empty space of the board and store the coordinates in x_coord and y_coord
void find_empty_space(const int board[X][Y], int* x_coord, int* y_coord) {
    int found = -1;

    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (board[i][j] == 0) {
                *x_coord = i;
                *y_coord = j;
                found = 1;
                break;
            }
        }
        if (found > 0) {
            break;
        }
    }
    if (found < 0) {
        *x_coord = -1;
        *y_coord = -1;
    }
}

//check_board
//parameters:   board is a matrix representing the playable board, player is a win_state enumerable, cpu is a win_state enumerable
//returns:      none
//this function checks the board to see if the player or cpu has won the game, will also decide if a draw has occurred
void check_board(const int board[X][Y],enum win_state* player,enum win_state* cpu) {
    int empty_x;
    int empty_y;

    for (int i = 0; i < X; i++) {
        if (board[i][0] == 1 && board[i][1] == 1 && board[i][2] == 1) {
            *player = game_won;
            break;
        }
        if (board[i][0] == 2 && board[i][1] == 2 && board[i][2] == 2) {
            *cpu = game_won;
            break;
        }
        if (board[0][i] == 1 && board[1][i] == 1 && board[2][i] == 1) {
            *player = game_won;
            break;
        }
        if (board[0][i] == 2 && board[1][i] == 2 && board[2][i] == 2) {
            *cpu = game_won;
            break;
        }
    }
    if (*player != game_won || *cpu != game_won) {
        if (board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1) {
            *player = game_won;
        }
        if (board[0][0] == 2 && board[1][1] == 2 && board[2][2] == 2) {
            *cpu = game_won;
        }
    }
    if (*player != game_won || *cpu != game_won) {
        find_empty_space(board, &empty_x, &empty_y);
        if (empty_x < 0 || empty_y < 0) {
            *player = game_draw;
            *cpu = game_draw;
        }
    }
}

//easy_bot_decision
//parameters:   board is a matrix representing the playable board
//returns:      none
//this function decides a move randomly, if there is a conflict it will try random choices 4 times
//if there is still a conflict, the function will simply find the first empty space and make that move
//or do nothing if there is no empty spaces available
void easy_bot_decision(int board[X][Y]) {
    int cpu_x = rand()%3;
    int cpu_y = rand()%3;
    int attempts = 0;
    while (board[cpu_x][cpu_y] != 0) {
        fprintf(stderr, "Conflict found at AI choice: %d %d, recalculating...\n",
                cpu_x, cpu_y);
        if (attempts <= 3) {
            cpu_x = rand()%3;
            cpu_y = rand()%3;
            attempts += 1;
        }
        else {
            find_empty_space(board, &cpu_x, &cpu_y);
            if (cpu_x < 0 || cpu_y < 0) {
                return;
            }
        }
    }
    fprintf(stderr, "AI move: %d %d\n", cpu_x, cpu_y);
    board[cpu_x][cpu_y] = 2;
}

//hard_bot_minimax
//parameters: board is a matrix representing the playable board, depth is an integer
//            representing the depth of the decision tree, maximizing is an enum value
//            which can either be maximize or minimize
//returns:    int representing the best score evaluated
//this function is a minimax implementation for the hard bot
int hard_bot_minimax(int board[X][Y], int depth, enum max_min maximizing) {
    int score = 0;
    int best;
    int next_move;
    enum win_state test_player = not_won;
    enum win_state test_cpu = not_won;

    check_board(board, &test_player, &test_cpu);

    if (test_cpu == game_won) {
        score = (score + 10) - depth;
        return score;
    }

    if (test_player == game_won) {
        score = (score - 10) + depth;
        return score;
    }

    if (test_player == game_draw || test_cpu == game_draw) {
        return score;
    }

    if (maximizing) {
        best = INT_MIN;

        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 2;
                    next_move = hard_bot_minimax(board, depth+1, !maximizing);
                    best = get_max_score(best, next_move);
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
    else {
        best = INT_MAX;

        for (int i = 0; i < X; i++) {
            for (int j = 0; j < Y; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 1;
                    next_move = hard_bot_minimax(board, depth+1, !maximizing);
                    best = get_min_score(best, next_move);
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

//hard_bot_decision
//parameters:   board is a matrix representing the playable board
//returns:      none
//this function will use a minimax implementation to get the best move for the given board
void hard_bot_decision(int board[X][Y]) {
    int best = INT_MIN;
    int move_score;
    int move_x = -1;
    int move_y = -1;

    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            if (board[i][j] == 0) {
                board[i][j] = 2;
                move_score = hard_bot_minimax(board, 0, minimize);
                board[i][j] = 0;

                if (move_score > best) {
                    move_x = i;
                    move_y = j;
                    best = move_score;
                }
            }
        }
    }
    fprintf(stderr, "AI move: %d %d\n", move_x, move_y);
    board[move_x][move_y] = 2;
}

//bot_move
//parameters:   board is a matrix representing the playable board, diff_choice is a pointer to the difficulty choice string
//returns:      none
//this function makes an AI decision depending on difficulty (when implemented) and places the move on the board
void bot_move(int board[X][Y], const char* diff_choice) {
    if (strcmp("Easy", diff_choice) == 0) {
        easy_bot_decision(board);
    }
    else if (strcmp("Hard", diff_choice) == 0) {
        hard_bot_decision(board);
    }
}