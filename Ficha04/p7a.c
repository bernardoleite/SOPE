#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int pid;
    int status;
    int i;
    int pids[argc - 1];

    for (i = 1; i < argc; i++) {
        pid = fork();
        if (pid == 0) {
            char* args[] = { argv[i] };
            //abort();
            execlp(args[0], args[0], NULL);
            exit(0);
        }
        else {
            pids[i - 1] = pid;
        }
    }

    for (i = 1; i < argc; i++) {
        pid = wait(&status);
        if (pid != -1) {
            if (WIFEXITED(status) != 1)
            {
				printf("Child terminated anormally\n");
				
                for (int i = 0; i < (argc - 1); i++) {
                    kill(pids[i], SIGKILL);
                }
                    exit(0);
			}
            else
                printf("Child with PID=%d terminated with exit code %d\n", pid, WEXITSTATUS(status));
        }
    }

    exit(0);
}