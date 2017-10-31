#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TTTGame.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:%s <difficulty>\n", argv[0]);
        printf("Difficulties: Easy or Hard\n");
        exit(-1);
    }
    else {
        char* diff_choice = argv[1];
        int board[X][Y];
        enum win_state player = not_won;
        enum win_state cpu = not_won;
        int move_x;
        int move_y;
        int valid_move;

        if (strcmp("Easy", diff_choice) != 0 && strcmp("Hard", diff_choice) != 0) {
            printf("Unrecognized difficulty level chosen.\n");
            exit(-1);
        }
        if (strcmp("Easy", diff_choice) == 0) {
            srand(time(NULL));
        }

        printf("Player is: %c , Difficulty: %s\n", 'X', diff_choice);
        board_init(board);
        do {
            print_board(board);
            do {
                printf("\nenter coordinates of desired play (format: X Y): ");
                scanf("%d %d", &move_x, &move_y);
                move_x -= 1;
                move_y -= 1;
                printf("move coordinates: %d %d\n", move_x, move_y);
                valid_move = update_board(board, move_x, move_y);
            }while (valid_move < 0);
            bot_move(board, diff_choice);
            fprintf(stderr, "AI move received\n");
            check_board(board, &player, &cpu);
            fprintf(stderr, "Game conditions: %d %d\n", player, cpu);
        }while (player == not_won && cpu == not_won);

        if (player == game_won) {
            printf("You won the game!\n");
        }
        else if (cpu == game_won) {
            printf("You lost!\n");
        }
        else {
            printf("Draw!\n");
        }
        exit(0);
    }
}