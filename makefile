CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude -O2 -std=gnu18 -g

OBJDIR=obj
SRCDIR=src

LIBS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

_OBJ = main.o bullets.o clean.o init.o assets.o intro.o handler.o menu.o levels.o automata.o render.o scores.o leaderboard.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(LIBS)

Shmalaga:	$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJDIR)/*.o
