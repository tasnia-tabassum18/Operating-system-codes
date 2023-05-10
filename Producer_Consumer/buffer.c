// TODO add error condition if buffer overflow

#include "buffer.h"
#include <semaphore.h>
#include <semaphore.h>
#include <pthread.h>

/* the buffer */
buffer_item buffer[BUFFER_SIZE];
size_t buff_head, buff_tail;

/* synch tools */
sem_t full, empty;
pthread_mutex_t lock;

void initialise_buffer() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    buff_head = buff_tail = 0;
}

void destroy_buffer() {
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&lock);
}

int insert_item( buffer_item item ) {
    /* insert item into buffer
    return 0 if successful, otherwise
    return -1 indicating an error condition */
    int retval = 0;

    sem_wait(&empty);
    pthread_mutex_lock(&lock);

    // if not overflow,
    if ( buff_tail < BUFFER_SIZE )  {
        buffer[buff_tail] = item;                               // add item
        buff_tail = (buff_tail + 1) % (BUFFER_SIZE + 1);        // increment buffer tail
    } else {
        retval = 1;
    }

    pthread_mutex_unlock(&lock);
    sem_post(&full);

    return retval;
}

int remove_item( buffer_item *item ) {
    /* remove an object from buffer placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */
    int retval = 0;

    sem_wait(&full);
    pthread_mutex_lock(&lock);

    // if not overflow,
    if ( buff_head < BUFFER_SIZE )  {
        *item = buffer[buff_head];
        buff_head = (buff_head + 1) % (BUFFER_SIZE + 1);
    } else {
        retval = 1;
    }

    pthread_mutex_unlock(&lock);
    sem_post(&empty);

    return retval;
}


