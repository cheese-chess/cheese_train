#ifndef __CHEESE_H__
#define __CHEESE_H__

extern int magic_table[6];

int cheese_eval(char *board, int turn);
int cheese_move(char *board, int *move, int do_move, int turn, int layers);

#endif
