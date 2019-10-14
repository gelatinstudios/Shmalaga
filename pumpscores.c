
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "scores.h"

int main(void) {
        srand(time(0));
        Score scores[10] = {0};
        Score score = {0};
        for (size_t i = 0; i < 10; ++i) {
                for(size_t j = 0; j < 3; ++j)
                        score.name[j] = (rand() % ('Z' - 'A')) + 'A';

                score.val = rand() % 10000;
                scores_update(scores, score);
        }

        print_scores(scores);
}
