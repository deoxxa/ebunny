CFLAGS := -I/usr/include/SDL2 -Werror -Wall

all: main_lcd main_sdl

main_lcd: main_lcd.o display.o image.o game.o sprites.o pcd8544.o
	$(CC) -o $@ $^ -lwiringPi -pthread

main_sdl: main_sdl.o display.o image.o game.o sprites.o
	$(CC) -o $@ $^ -lSDL2

clean:
	rm -f main_lcd main_sdl *.o
