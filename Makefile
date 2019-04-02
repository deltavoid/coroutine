

CC       := cc


MAIN_NAME := main
MAIN_SRCS := src/main.c src/coroutine.c src/switch.S



.PHONY: all build run clean

all: run


build: $(MAIN_NAME)

$(MAIN_NAME): $(MAIN_SRCS)
	$(CC) -o $@ $^


run: build
	./$(MAIN_NAME)

clean:
	rm  $(MAIN_NAME)

