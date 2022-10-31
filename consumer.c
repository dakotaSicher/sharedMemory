#include <string.h>
#include "shm_sq.h"

int main()
//int argc, char *argv[])
{
/*
    if (argc != 3) {
        fprintf(stderr, "Usage: %s /shm-path string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *shmpath = argv[1];
    char *string = argv[2];
    size_t len = strlen(string);

    if (len > BUF_SIZE) {
        fprintf(stderr, "String is too long\n");
        exit(EXIT_FAILURE);
    }
*/
    /* Open the existing shared memory object and map it
       into the caller's address space. */

    int fd = shm_open("/shmem", O_RDWR, 0);
    if (fd == -1)
        errExit("shm_open");

    struct shmbuf *shmp = mmap(NULL, sizeof(*shmp),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

// wait for producer to produce the first items
    if (sem_wait(&shmp->sem2) == -1)
        errExit("sem_wait");

    while(!(shmp->empty)){

        int num = shmp->buf[shmp->out];
        printf("The square of %i = %i \n", num, num*num);

        shmp->out = ((shmp->out)+1) % BUF_SIZE;
        shmp->full = false;   //if an item has been consumed the queue is not full
        if(shmp->out == shmp->in) shmp->empty = true; //if the queue is not full and out == in then it must be empty

        if(shmp->empty){ // only after the queue is empty do we want to pass back to the producer
            if (sem_post(&shmp->sem1) == -1) // inform produce that the consumer is done
                errExit("sem_post");

            if (sem_wait(&shmp->sem2) == -1) // wait for the producer
                errExit("sem_wait");
        }
    }


    exit(EXIT_SUCCESS);
}



