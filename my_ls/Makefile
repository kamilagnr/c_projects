CC := gcc
CFLAGS := -Wall -Wextra -Werror -g3 -fsanitize=address

all: my_ls

my_ls: main.o my_ls.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $<

my_ls.o: my_ls.c my_ls.h
	$(CC) $(CFLAGS) -c $< 

clean:
	rm *.o my_ls
