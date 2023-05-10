/*
    Write a program that creates three child processes and three pipes to communicate with
    each process. Each child process reads from different serial line (keyboard) and sends
    the characters read back to the parent process through a pipe. The parent process outputs
    all characters received on the console. A child terminates when two newline characters
    are received consecutively. The parent terminates after all three children have
    terminated. (hint: send-pipe and receive-pipe primitives can be used)
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int const READ = 0;
int const WRITE = 1;
int const PROCESS_NUM = 3;
int const BUFFER_SIZE = 10;

void child_task( int fd[PROCESS_NUM][2], int child_id );

int main( int argc, char *argv[] ) {

    // Variables
    int i, j;                       // for loops
    pid_t pids[PROCESS_NUM];        // child processes to be forked
    int fd[PROCESS_NUM][2];

    for (i = 0; i <= PROCESS_NUM; i++) { 
        if (pipe(fd[i]) == -1) {
            printf("Error with creating pipe %d\n", i);
            return 1;
        }
    }

    // Fork first child process
    pids[0] = fork();
    if (pids[0] == 0) {
        child_task(fd, 1);                   // Child 1
        exit(0);

    } else {
        // Fork second child
        pids[1] = fork();
        if (pids[1] == 0){ 
            child_task(fd, 2);               // Child 2
            exit(0);

        } else {
            // Fork third child
            pids[2] = fork();
            if (pids[2] == 0 ) {
                child_task(fd, 3);           // Child 3
                exit(0);

            } else {
                printf("Parent Started");
                char message_buffer[BUFFER_SIZE];

                close(fd[0][WRITE]); close(fd[1][WRITE]); close(fd[2][WRITE]);
                while( wait(NULL) > 0 ) {
                    
                    if ( read(fd[0][READ], &message_buffer, BUFFER_SIZE) ) {
                        printf("child1: %s\n", message_buffer);
                    }
                    if ( read(fd[1][READ], &message_buffer, BUFFER_SIZE) ) {
                        printf("child2: %s\n", message_buffer);
                    }
                    if ( read(fd[2][READ], &message_buffer, BUFFER_SIZE) ) {
                        printf("child3: %s\n", message_buffer);
                    }
                }
                close(fd[0][READ]); close(fd[1][READ]); close(fd[2][READ]);
                
                printf("\nChildren Complete\n");
            }
        }
    }
    return 0;
}


void child_task( int fd[PROCESS_NUM][2], int child_id ) {

    printf("Child %d task started\n", child_id);
    
    int strike = 0;                      // Counts consecutive \n
    char input_buffer[BUFFER_SIZE];      // Stores keyboard input
    int input_size;                      // Size of input string

    close(fd[child_id-1][READ]);

    while (1) {
        // Get keyboard input
        printf("Input to child %d: ", child_id);
        fgets(input_buffer, BUFFER_SIZE, stdin);
        input_size = strlen(input_buffer) + 1;

        // Write input string size and input string to pipe
        if ( write(fd[child_id][WRITE], &input_size, sizeof(int)) < 0 ) {
            exit( 10 * child_id );
        }
        if ( write(fd[child_id][WRITE], &input_buffer, (sizeof(char) * input_size)) < 0 ) {
            exit( 10 * child_id );
        }

        // Break out if two consecutive newlines (i.e. empty string) received
        input_size = (input_size == 1) ? 0 : input_buffer;
        if (input_buffer[input_size] == '\n') {
            strike++;
            if (strike > 1) {
                break;
            }
        } else {
            strike = 0;
        }
    }

    close(fd[child_id-1][WRITE]);
    printf("Child %d terminating\n", child_id);
    exit(0);
}