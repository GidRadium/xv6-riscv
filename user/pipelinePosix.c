#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "not enough arguments\n");
        exit(1);
    }

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        if (close(pipefd[1]) != 0) {
            perror("close");
            exit(1);
        }

        char buff[32];
        int readBytes;
        while ((readBytes = read(pipefd[0], buff, sizeof(buff))) > 0) {
            if (write(1, buff, readBytes) != readBytes) {
                perror("write");
                if (close(pipefd[0]) != 0) {
                    perror("close");
                    exit(1);
                }
                exit(1);
            }
        }

        if (readBytes < 0) {
            perror("read");
            if (close(pipefd[0]) != 0) {
                perror("close");
            }
            exit(1);
        }

        if (close(pipefd[0]) != 0) {
            perror("close");
            exit(1);
        }

        exit(0);
    }

    if (close(pipefd[0]) != 0) {
        perror("close");
        if (close(pipefd[1]) != 0) {
            perror("close");
        }
        if (wait(0) < 0) {
            perror("wait");
        }
        exit(1);
    }

    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        int offset = 0;

        while (len > 0) {
            int writed = write(pipefd[1], argv[i] + offset, len);
            if (writed < 0) {
                perror("write");
                if (close(pipefd[1]) != 0) {
                    perror("close");
                }
                if (wait(0) < 0) {
                    perror("wait");
                }
                exit(1);
            }
            len -= writed;
            offset += writed;
        }

        if (write(pipefd[1], "\n", 1) != 1) {
            perror("write");
            if (close(pipefd[1]) != 0) {
                perror("close");
            }
            if (wait(0) < 0) {
                perror("wait");
            }
            exit(1);
        }
    }

    if (close(pipefd[1]) != 0) {
        perror("close");
        if (wait(0) < 0) {
            perror("wait");
        }
        exit(1);
    }

    if (wait(0) < 0) {
        perror("wait");
        exit(1);
    }

    exit(0);
}
