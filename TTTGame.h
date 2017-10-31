#ifndef TTTGame_h
#define TTTGame_h

enum win_state {game_won = 1, not_won = -1, game_draw = 2};
enum max_min {maximize = 1, minimize = -1};
static const int X = 3;
static const int Y = 3;

void board_init(int board[X][Y]);
char get_symbol(int player_value);
void print_board(const int board[X][Y]);
int update_board(int board[X][Y],int player_x, int player_y);
int get_max_score(int score_a, int score_b);
int get_min_score(int score_a, int score_b);
void find_empty_space(const int board[X][Y], int* x_coord, int* y_coord);
void check_board(const int board[X][Y],enum win_state* player,enum win_state* cpu);
void easy_bot_decision(int board[X][Y]);
int hard_bot_minimax(int board[X][Y], int depth, enum max_min maximizing);
void hard_bot_decision(int board[X][Y]);
void bot_move(int board[X][Y], const char* diff_choice);

#endif