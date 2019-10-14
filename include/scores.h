#ifndef SCORES_H
#define SCORES_H

typedef struct {
        char name[3];
        unsigned val;
} Score;

void load_scores(Score[10]);
void print_scores(Score[10]);
void scores_update(Score[10], Score);

#endif
