/*
The reader writer problem: A number of readers may simultaneously be reading from a
file. Only one writer at a time may write to file, and no reader can be reading while a
writer is writing. Using semaphores, Write program with solution to the reader writers
problem that gives priority to writers.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t rw_mutex;
pthread_mutex_t counter_mutex;            // Controls read count
int read_count = 0;

void writer( int id ) {
    while (1) {
        sleep(2);       // for readability

        pthread_mutex_lock(&rw_mutex);

        /* writing is performed */
        printf("\n\tWriter %d writes", id);

        pthread_mutex_unlock(&rw_mutex);
    }
}

void reader( int id ) {
    while (1) {
        sleep(2);       // for readability

        pthread_mutex_lock(&counter_mutex);
        if(read_count == 1){
            pthread_mutex_lock(&rw_mutex);
        }
        read_count++;
        pthread_mutex_unlock(&counter_mutex);
        
        /* reading is performed */
        printf("\nReader %d reads", id);

        pthread_mutex_lock(&counter_mutex);
        read_count-- ;
        if( read_count==0){
            pthread_mutex_unlock(&rw_mutex);
        }
        pthread_mutex_unlock(&counter_mutex);
    }
} 


int main(){
    pthread_t read[10],write[5];

    pthread_mutex_init(&rw_mutex, NULL);
    pthread_mutex_init(&counter_mutex, NULL);

    int a[10] = {1,2,3,4,5,6,7,8,9,10}; // used for numbering the producer and consumer

    for(int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }


    return 0;
    
}