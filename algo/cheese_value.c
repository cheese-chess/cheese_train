#ifdef CHEESE_ALGO_VALUE

#include <ctype.h>
#include <chess.h>
#include <stdlib.h>
#include <string.h>
#include <cheese.h>

const int cheese_table[] = {
  11, 12, 13, 14, 14, 13, 12, 11,
  12, 13, 14, 15, 15, 14, 13, 12,
  13, 14, 15, 16, 16, 15, 14, 13,
  14, 15, 16, 17, 17, 16, 15, 14,
  14, 15, 16, 17, 17, 16, 15, 14,
  13, 14, 15, 16, 16, 15, 14, 13,
  12, 13, 14, 15, 15, 14, 13, 12,
  11, 12, 13, 14, 14, 13, 12, 11
};

const int burger_king_black_table[] = {
  18, 18, 19, 20, 20, 19, 18, 18,
  17, 17, 18, 19, 19, 18, 17, 17,
  16, 16, 17, 18, 18, 17, 16, 16,
  15, 15, 16, 17, 17, 16, 15, 15,
  14, 14, 15, 16, 16, 15, 14, 14,
  13, 13, 14, 15, 15, 14, 13, 13,
  12, 12, 13, 14, 14, 13, 12, 12,
  11, 11, 12, 13, 13, 12, 11, 11
};

const int burger_king_white_table[] = {
  11, 11, 12, 13, 13, 12, 11, 11,
  12, 12, 13, 14, 14, 13, 12, 12,
  13, 13, 14, 15, 15, 14, 13, 13,
  14, 14, 15, 16, 16, 15, 14, 14,
  15, 15, 16, 17, 17, 16, 15, 15,
  16, 16, 17, 18, 18, 17, 16, 16,
  17, 17, 18, 19, 19, 18, 17, 17,
  18, 18, 19, 20, 20, 19, 18, 18
};

static int cheese_value(char piece, int pos) {
  int black = 0;

  if (piece == ' ') return 0;
  if (piece >= 'a') piece -= 32, black = 1;

  switch (piece) {
    case 'P':
      return 1 * cheese_table[pos];
    case 'B':
      return 3 * cheese_table[pos];
    case 'N':
      return 3 * cheese_table[pos];
    case 'R':
      return 5 * cheese_table[pos];
    case 'Q':
      return 9 * cheese_table[pos];
    case 'K':
      if (black) return 1000 * burger_king_black_table[pos];
      else return 1000 * burger_king_white_table[pos];
  }

  return 0;
}

int cheese_eval(char *board, int turn) {
  int score = 0;

  for (int i = 0; i < 64; i++) {
    if (chess_turn(board[i]) == CHESS_NONE) {
      continue;
    } else if (chess_turn(board[i]) == turn) {
      score += cheese_value(board[i], i);
    } else {
      score -= cheese_value(board[i], i);
    }
  }

  return score;
}

int cheese_move(char *board, int turn, int layers) {
  int **moves = chess_get_moves(board, turn);

  if (!moves) return -10000000;

  int move_cnt = 0;

  while (moves[move_cnt] != NULL) move_cnt++;

  int best_move = -1;
  int best_score = -10000000;

  char *board_clone = malloc(64);

  for (int i = 0; i < move_cnt; i++) {
    memcpy(board_clone, board, 64);

    chess_move(board_clone, moves[i]);

    int score = cheese_eval(board_clone, turn);

    if (layers > 0) {
      score += cheese_move(board_clone, 1 - turn, layers - 1);
    }

    if (score > best_score) {
      best_move = i;
      best_score = score;
    }
  }

  free(board_clone);

  if (best_move >= 0) chess_move(board, moves[best_move]);

  chess_free(moves);

  return best_score;
}

#endif
