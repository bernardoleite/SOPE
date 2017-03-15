// PROBLEMA 1 DA FOLHA 2

#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_PASS_LEN 20 

int main (void) {
	struct termios oldterm, newterm;
	char pass[MAX_PASS_LEN], checkPass[MAX_PASS_LEN], ch, echo = '*';
	int i = 0;	
	checkPass[1] = 'l';
	write(STDOUT_FILENO, "\nPassword? ", 11);

	tcgetattr(STDIN_FILENO, &oldterm);
	newterm=oldterm;
	newterm.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &newterm);
	
	// read pass and write *
	while (i < MAX_PASS_LEN && read(STDIN_FILENO, &ch, 1) && ch != '\n') {
		pass[i++] = ch;
		write(STDOUT_FILENO, &echo, 1);
	}
	pass[i] = 0;

	tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
	
	//write pass	
	write(STDOUT_FILENO, "\n\nPassword: ", 12);
	write(STDOUT_FILENO, pass, strlen(pass));
	write(STDOUT_FILENO, "\n", 2);
	return 0;
}
