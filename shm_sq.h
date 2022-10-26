#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

//#define BUF_SIZE 1024   /* Maximum size for exchanged string */
#define BUF_SIZE 2

/* Define a structure that will be imposed on the shared
   memory object */

struct shmbuf {
    sem_t  sem1;            /* POSIX unnamed semaphore */
    sem_t  sem2;            /* POSIX unnamed semaphore */
//    size_t cnt;
//    char   buf[BUF_SIZE];   /* Data being transferred */
    int    buf[BUF_SIZE];
    int    in, out;
    bool   prod;
};
