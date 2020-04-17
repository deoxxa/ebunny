CFLAGS := -I/usr/include/SDL2 -Werror -Wall
LDLIBS := -lSDL2

all: main_lcd main_sdl

main_lcd: image.o game.o sprites.o main_lcd.o
main_sdl: image.o game.o sprites.o main_sdl.o

clean:
	rm -f main_lcd main_sdl *.o
