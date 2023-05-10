#include <stdlib.h>
#include "buffer.h"

void *producer(void *param) {
    buffer_item item;
    while (1) {
        /* sleep for a random period of time */
        sleep( rand() % MAX_SLEEP_TIME + 1 );

        /* generate a random number */
        item = rand();
        if (insert_item(item))
            fprintf("report error condition");
        else
            printf("producer produced %d∖n",item);
}

void *consumer(void *param) {
    buffer item item;
    while (1) {
        /* sleep for a random period of time */
        sleep( rand() % MAX_SLEEP_TIME + 1 );
        
        if (remove_item(&item))
            fprintf("report error condition");
        else
            printf("consumer consumed %d∖n",item);
    }
}