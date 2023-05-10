/* buffer.h */
#define BUFFER_SIZE 5 
#define MAX_SLEEP_TIME 3

typedef int buffer_item;

int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param);
void *consumer(void *param);
void initialise_buffer();
void destroy_buffer();
