#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>

struct termios orig_termios;

void die(const char *s) {
	perror(s);
	exit(1);
}

void disable_raw_mode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disable_raw_mode);;

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(ICRNL | IXON | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enable_raw_mode();

	char c;
	while(1) {
		c = '\0';
		read(STDIN_FILENO, &c, 1);
		if(iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if(c == 'q') {
			break;
		}
	}
	return 0;
}
