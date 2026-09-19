#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("wrong argument count\n");
        exit(1);
    }
    int fork_status = fork();
    if (fork_status < 0) {
        printf("fork error\n");
        exit(1);
    }
    else if (fork_status == 0) {
        pause(100);
        exit(1);
    }
    else {
        int parent_pid = getpid();
        printf("parent PID: %d, child PID: %d\n", parent_pid, fork_status);
        if (*argv[1] == 'a') {
            int status;
            int child_pid = wait(&status);

            printf("child PID: %d, child exit code: %d\n", child_pid, status);

            exit(0);
        }
        else if (*argv[1] == 'b') {
            kill(fork_status);

            int status = 0;
            int child_pid = wait(&status);

            printf("child_PID: %d, status: %d\n", child_pid, status);
            exit(0);
        }
        else {
            printf("wrong argument\n");
            exit(1);
        }
    }
}
