OUTPUT = minesweeper

CC = gcc

SRCS = main.c minelib.c
OBJS = $(SRCS:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS)

test: $(OUTPUT)
	./$(OUTPUT)