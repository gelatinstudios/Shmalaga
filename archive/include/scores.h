#ifndef SCORES_H
#define SCORES_H

typedef struct {
        char name[3];
        unsigned val;
} Score;

void load_scores(Score[]);
void print_scores(Score[]);
void scores_update(Score[], Score);

#endif
