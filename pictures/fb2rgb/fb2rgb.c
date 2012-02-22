/*
 * Convert images from the rgb565 (mini2440 framebuffer) format to 
 * standard RGB, and the other way around.
 * 
 * Screenshot :
 * ssh petit cat /dev/fb0 | ./fb2rgb - - | convert -size 320x240 -depth 8 rgb:- fb.png
 * Display :
 * convert fb.png rgb:- | ./fb2rgb - - -r | ssh petit "cat - > /dev/fb0"
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char* filename;
void panic(const char* msg) {
	fprintf(stderr, "%s : %s\n", filename,  msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {
	int input, output;
	
	filename = argv[0];
	
	if (argc < 3)
		panic("Missing input or output paths");
	
	if (!strcmp(argv[1], "-"))
		input = STDIN_FILENO;
	else
		input = open(argv[1], O_RDONLY);
	if (input < 0)
		panic("Cannot open input");
	
	if (!strcmp(argv[2], "-"))
		output = STDOUT_FILENO;
	else
		output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (output < 0)
		panic("Cannot open output");
	
	unsigned short s;
	int i = 0;
	if (argc >= 4 && !(strcmp(argv[3], "-r")
			&& strcmp(argv[3], "--reverse"))) { // rgb2fb
		char c[3];
		while (read(input, c, 3) == 3) {
			s = (c[0] & (unsigned short)0xF8) << 8
				| (c[1] & (unsigned short)0xFC) << 3
				| c[2] >> 3;
			write(output, (char*)&s, 1);
			write(output, 1 + (char*)&s, 1);
			i += 2;
		}
	}
	else { // fb2rgb
		char c;
		while (read(input, (char*)&s, 2) == 2) {
			c = (s & 0xF800) >> 8;
			write(output, &c, 1);
			c = (s & 0x07E0) >> 3;
			write(output, &c, 1);
			c = (s & 0x001F) << 3;
			write(output, &c, 1);
			i += 3;
		}
	}
	fprintf(stderr, "%i bytes written\n", i);
	
	close(input);
	close(output);
	
	return EXIT_SUCCESS;
}
