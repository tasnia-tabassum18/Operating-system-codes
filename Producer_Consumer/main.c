#include "buffer.h"
#include <pthread.h>


int main(int argc, char *argv[]) {
    int sleep_time;
    size_t n_producer;
    size_t n_consumer;

    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    if (argc != 4 ) {
        // Abort
        printf("Aborting: must receive 3 parameters: sleep_time, n_producer, n_consumer.");
        return 1;
    } else {
        // Parse arguments
        sleep_time = atoi(argv[1]);
        n_producer = atoid(argv[2]);
        n_consumer = atoid(argv[3]);
    }


    /* 2. Initialize buffer */
    initialise_buffer();


    /* 3. Create producer thread(s) */
    pthread_t *producers = malloc(n_producer * sizeof(pthread_t)); //malloc dynamically allocates memory
    int *producer_id = malloc(n_producer * sizeof(pthread_t));

    for(size_t i = 0 ; i != n_producer; ++i) {
        producer_id[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_id[i]);
    }


    /* 4. Create consumer thread(s) */
    pthread_t *consumers = malloc(n_consumer * sizeof(pthread_t));
    int *consumer_id = malloc(n_consumer * sizeof(pthread_t));

    for(size_t i = 0; i != n_consumer; ++i) {
        consumer_id[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_id[i]);
    }


    /* 5. Sleep */
    printf("Sleep for %u second(s) before exit.\n", sleep_time);
    sleep(sleep_time);


    /* 6. Exit */
    for(size_t i = 0; i != n_producer; ++i) {
        pthread_cancel(producers[i]);
        pthread_join(producers[i], NULL);
    }

    sleep(3);   // a short buffer period to consume packets

    for(size_t i = 0; i != n_consumer; ++i) {
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }

    free(producers);
    free(producer_id);
    free(consumers);
    free(consumer_id);
    destroy_buffer();

    return 0;
}