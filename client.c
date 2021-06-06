#include <raylib.h>
#include <stdlib.h>
#include <cheese.h>
#include <chess.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define DEPTH 3

#define NUM_THREADS 2

int get_score(int *table) {
  char board[65] = \
    "rnbqkbnr" \
    "pppppppp" \
    "        " \
    "        " \
    "        " \
    "        " \
    "PPPPPPPP" \
    "RNBQKBNR" \
  ;

  for (int i = 0; i < 50; i++) {
    int white_pieces = 0;

    for (int j = 0; j < 64; j++) {
      int turn = chess_turn(board[j]);

      if (turn == CHESS_WHITE) white_pieces++;
    }

    if (chess_ended(board)) break;

    if (white_pieces <= 5) {
      memcpy(magic_table, table + 12, sizeof(int) * 6);
    } else if (white_pieces <= 11) {
      memcpy(magic_table, table + 6, sizeof(int) * 6);
    } else {
      memcpy(magic_table, table + 0, sizeof(int) * 6);
    }

    cheese_move(board, NULL, 1, CHESS_WHITE, DEPTH);

    if (chess_ended(board)) break;
    aa_cheese_move(board, NULL, 1, CHESS_BLACK, DEPTH);
  }

  int black_pieces = 0, white_pieces = 0;

  for (int j = 0; j < 64; j++) {
    int turn = chess_turn(board[j]);

    if (turn == CHESS_WHITE) white_pieces++;
    if (turn == CHESS_BLACK) black_pieces++;
  }

  int value = cheese_eval(board, CHESS_WHITE);

  // printf("\"%s\": %d\n", board, (value * 100) / (black_pieces + white_pieces));

  return (value * 100) / (black_pieces + white_pieces);
}

int get_avg_score(int *table) {
  int score = 0;

  for (int i = 0; i < 3; i++) {
    score += get_score(table);
  }

  return score / 3;
}

int table[] = {
  1, 4, 5, 6, 11, 1000,
  1, 3, 3, 6, 11, 1000,
  1, 4, 4, 6, 11, 1000
};

int best_score;

void *choo_choo(void *id) {
  int new_table[18];

  for (;;) {
    memcpy(new_table, table, sizeof(int) * 18);

    int count = 1 + rand() % 5;

    while (count) {
      int index = rand() % 18;
      int dir = (rand() % 2) * 2 - 1;

      if (new_table[index] + dir <= 0) continue;

      new_table[index] += dir;
      count--;
    }

    int score = get_avg_score(table);

    printf("[%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d]: %d\n",
           new_table[0], new_table[1], new_table[2], new_table[3], new_table[4], new_table[5],
           new_table[6], new_table[7], new_table[8], new_table[9], new_table[10], new_table[11],
           new_table[12], new_table[13], new_table[14], new_table[15], new_table[16], new_table[17],
           score);

    if (score > best_score) {
      memcpy(table, new_table, sizeof(int) * 6);
      best_score = score;

      printf("New best score: %d\n", best_score);
    }
  }
}

int main(int argc, char *argv[]) {
  best_score = get_avg_score(table);

  pthread_t threads[NUM_THREADS];

  for (int i = 0 ; i < NUM_THREADS ; i++) {
    int t = pthread_create(&threads[i], NULL, choo_choo, NULL);

    if (t != 0) {
      exit(1);
    }
  }

  for (;;);

  return 0;
}
