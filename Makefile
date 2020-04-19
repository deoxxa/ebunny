CFLAGS := -I/usr/include/SDL2 -Werror -Wall

all: main_lcd main_sdl

main_lcd: image.o game.o sprites.o pcd8544.o main_lcd.o
	$(CC) -o $@ $^ -lwiringPi -pthread

main_sdl: image.o game.o sprites.o main_sdl.o
	$(CC) -o $@ $^ -lSDL2

clean:
	rm -f main_lcd main_sdl *.o
