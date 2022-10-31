#Dakota Sicher
#Operating Systems
#shared memory homework assignment
#
#compile: gcc producer.c -pthread -lrt -o producer
#        gcc consumer.c -pthread -lrt -o producer
#
#run:     ./producer & ./consumer
#
#Takes no input or command line arguements for simplicity sake
#instead i have a simple const int in the producer to dictate the number of loops
#
#I used the linux manual page provided by Dr. Guan for the shm_open() function and associated functions
#
#shm_open() creates a new memory space and assigns it the provided path
#the memory address is resized to the required memory size of the shared memory struct in shm_sq.h 
#finaly an instance of the struct is created within this memory space
#
#I implimented the semephores as they did in the example with a semephore for each of the processes
#
#sem1:  producer waits - if sem1 == 0 the producer sleeps until sem1 > 0 and then decriments and continues
#                        if sem1 > 0 then it immediately decriments and continues
#       consumer posts - incriments
#
#sem2:  producer posts - increments
#       consumer waits - if sem2 == 0 then the consumer waits until it can increments 
#                        if sem2 > 0 then it immediately increments and proceeds
#
#the producer simply uses a for loop to insert sequential numbers into the buf(within the shmbuf struct) until the buff is full or the loop is on its last iteration
#then it posts on sem2 and waits and sem1
#
#the consumer starts by waiting on sem1 so that the producer is able to go first, then it enters a while loop that continues as long as the buf is not empty when passed back to the consumer
#an empty buf when control is passed back to consumer signals that the producer has finished producing and is exiting, which in turn causes the consumer to exit the while loop and exit as well
#
#
#all the consumer does is take the number from the buf and square them, and then print them out.
#
#while i could have added cmd line arguments or console inputs, it was not required by the assignment, and i tried to keep things simple and to the point which was the shared memory and semephores aspects of the programs

