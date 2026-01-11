LIB_NAME := libvecmath.a

CC := clang

CFLAGS :=-std=c11 -O3 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)


.PHONY = clean

$(LIB_NAME): $(OBJ)
	rm ./$(LIB_NAME)
	ar -cvq $(LIB_NAME) *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	rm *.o *.a
