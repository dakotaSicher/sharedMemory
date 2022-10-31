#include <ctype.h>
#include "shm_sq.h"

int main()
//int argc, char *argv[])
{
/*
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *shmpath = argv[1];
*/
    /* Create shared memory object and set its size to the size
       of our structure. */

//    shm_unlink("/shmem");

    int fd = shm_open("/shmem", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        errExit("ftruncate");

    /* Map the object into the caller's address space. */

    struct shmbuf *shmp = mmap(NULL, sizeof(*shmp),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmap");

    /* Initialize semaphores as process-shared, with value 0. */

    if (sem_init(&shmp->sem1, 1, 0) == -1)
        errExit("sem_init-sem1");
    if (sem_init(&shmp->sem2, 1, 0) == -1)
        errExit("sem_init-sem2");


    //set buf in and out to begining of array
    shmp->in = 0;
    shmp->out= 0;
    shmp->empty = true;
    shmp->full = false;
    const int total = 5;

    for(int i=1; i<=total; ++i){
        //add value to next element
        shmp->buf[shmp->in] = i;
        shmp->in = ((shmp->in)+ 1) % BUF_SIZE;
        //if an item has just been produced than the queue cannot be empty
        shmp->empty = false;
        //if the queue is not empty and in == out then it must be full
        if (shmp->in == shmp->out) shmp->full = true;

        if(shmp->full || i == total){ // pass to consumer if the queue is full OR the producer is done producing
            if (sem_post(&shmp->sem2) == -1) // inform consumer that producer is done accessing memory
                errExit("sem_post");
//        printf("Producer waits\n");

            if (sem_wait(&shmp->sem1) == -1) // wait on consumer to finish
                errExit("sem_wait");
//        printf("producer proceeds\n");

        }
    }
    if (sem_post(&shmp->sem2) == -1)
        errExit("sem_post");

    /* Unlink the shared memory object. Even if the peer process
       is still using the object, this is okay. The object will
       be removed only after all open references are closed. */

    shm_unlink("/shmem");

    exit(EXIT_SUCCESS);
}
