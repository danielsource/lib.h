warnings = -Wall -Wextra -Wpedantic -Wshadow -Wconversion

CC = clang
RM ?= rm -f
#CFLAGS = -std=c99 $(warnings) -ggdb -O0 -DDEBUG -fanalyzer -fsanitize=address,leak,undefined -I ..
CFLAGS = -std=c99 $(warnings) -ggdb -O0 -DDEBUG -fsanitize=address,leak,undefined -I ..
LDFLAGS =
