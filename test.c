
#include <stdio.h>

int main(void) {
        unsigned score = 500;
        size_t i = 0;
        while (score) {
                score = 500 / pow(2, i);
                printf("%u\n", score);
                ++i;
        }
}
