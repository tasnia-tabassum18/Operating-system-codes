/*
The reader writer problem: A number of readers may simultaneously be reading from a
file. Only one writer at a time may write to file, and no reader can be reading while a
writer is writing. Using semaphores, Write program with solution to the reader writers
problem that gives priority to writers.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int readcount, writecount = 0;
sem_t reader_sem, writer_sem,
      read_count_sem, writer_count_sem, 
      z;

void reader(){
    while(1) {
        wait(z); 
            wait(reader_sem); 
                wait(read_count_sem);
                if ( ++readcount == 1 ) {
                    wait(writer_sem);
                }
                signal(read_count_sem);
            signal(reader_sem);
        signal(z);
        
        printf("Reading...");

        wait(read_count_sem);
        if ( --readcount == 0 ) {
            signal(writer_sem);
        }
        signal(read_count_sem);
    }
}

void writer(){
    while(1) {
        wait(writer_count_sem);
        if ( ++writecount == 1 ) {
            wait(reader_sem);
        }
        signal(writer_count_sem);

        wait(writer_sem);
        printf("Writing...");
        signal(writer_sem);

        wait(writer_count_sem);
        if ( --writecount == 0 ) {
            signal(reader_sem);
        }
        signal(writer_count_sem);
    }
}


int main(){
    sem_init(&reader_sem,0,1);
    sem_init(&writer_sem,0,1);
    sem_init(&read_count_sem,0,1);
    sem_init(&writer_count_sem,0,1);
    sem_init(&z,0,1);

    int p = fork();
    if(p) reader; // assume multiple instances
    else  writer; // assume multiple instances
}