
#include <stdio.h>

typedef struct {
	char name[3];
	unsigned val;
} Score;	

int main(void) {
	FILE *file = fopen("scores", "r");
	if(!file) {
		puts("error");
		return 1;
	}
	
	Score scores[10];
	fread(scores, sizeof(Score), 10, file);
	fclose(file);

	for(size_t i = 0; i < 10; ++i) 
		printf("%c%c%c:\t%u\n", scores[i].name[0] ? : '-', scores[i].name[1] ? : '-', scores[i].name[2] ? : '-', scores[i].val);
}
