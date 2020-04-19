CFLAGS := -I/usr/include/SDL2 -Werror -Wall

all: main_lcd main_sdl

main_lcd: main_lcd.o display.o image.o game.o sprites.o pcd8544.o
	$(CC) -o $@ $^ -lwiringPi -pthread

main_sdl: main_sdl.o display.o image.o game.o sprites.o
	$(CC) -o $@ $^ -lSDL2

.PHONY: clean
clean:
	rm -f main_lcd main_sdl *.o

.PHONY: install_lcd
install_lcd: main_lcd
	install -o root -g root main_lcd /usr/local/bin/ebunny
	install -o root -g root -d /usr/local/lib/systemd/system
	install -o root -g root -m 0644 ebunny.service /usr/local/lib/systemd/system/ebunny.service
