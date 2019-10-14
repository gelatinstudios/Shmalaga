#include "essentials.h"

void load_scores(Score scores[]) {
        FILE *file = fopen("scores", "r");
        if(!file) return;
        fread(scores, sizeof(Score), 10, file);
        fclose(file);
}

void print_scores(Score scores[]) {
        FILE *file = fopen("scores", "w");
        fwrite(scores, sizeof(Score), 10, file);
        fclose(file);
}

void scores_update(Score scores[], Score score) {
        assert(score.val > scores[9].val);
        size_t i = 0;
        while(score.val <= scores[i].val) ++i;
        for(size_t j = 8; j >= i && j < 9; --j)
                scores[j+1] = scores[j];
        scores[i] = score;
}
