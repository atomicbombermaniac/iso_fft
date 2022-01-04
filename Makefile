all:
	gcc -Wall -g -O0 -I/usr/include/SDL2 -I. -o iso_fft *.c -lpulse-simple -lpulse -lm -lSDL2 -lSDL2_image -lSDL2_mixer

clean:
	rm ./iso_fft