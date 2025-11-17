CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic

MAIN_EXEC = main

MAIN_SRC = main.c

all: $(MAIN_EXEC)

$(MAIN_EXEC): $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC)

clean:
	rm -f $(MAIN_EXEC)

package:
	zip NguyenPhucAnh_20225784.zip $(MAIN_SRC) Makefile
