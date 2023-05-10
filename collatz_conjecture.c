/*
The Collatz conjecture concerns what happens when we take any positive 
integer n and apply the following algorithm:

n  =  n/2          if n is even
n  =  3 × n + 1    if n is odd

The conjecture states that when this algorithm is continually applied, all positive integers
will eventually reach 1. For example, if n = 35, the sequence is
            35, 106, 53, 160, 80, 40, 20, 10, 5, 16, 8, 4, 2, 1

Write a C program using the fork () system call that generates this sequence in the child
process. 

The starting number will be provided from the command line. For example, if 8 is passed as a 
parameter on the command line, the child process will output 8, 4, 2, 1. Because the parent 
and child processes have their own copies of the data, it will be necessary for the child to 
output the sequence. Have the parent invoke the wait() call to wait for the child process to 
complete before exiting the program. Perform necessary error checking to ensure that a positive 
integer is passed on the command line.
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main( int argc, char *argv[] ) {

    // Check user input
    int n; sscanf( argv[1], "%i", &n );

    if ( !(argc > 1) && !(n > 0) ) {
        fprintf(stderr, "You must pass a positive integer as command line argument");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;

    } else if (pid == 0) {              // child process generates sequence
        printf(" %d", n);
        while (n != 1) {
            if (n % 2 == 0) {
                n = n / 2;
                printf(" %d", n);
            } else {
                n = (n*3)+1;
                printf(" %d ", n);
            }
        }
    } else {
        wait(NULL);                     // carent waits for child to complete
        printf("\nChild Complete\n");
    } 

    return 0;
}
