/* This example shows how condition variables can be used to control access
   of reads and writes to a buffer. This example can also be thought of as a
   producer/consumer problem, where the producer adds items to the buffer and
   the consumer removes items from the buffer.
   by Koren Ward - University of Wollongong
   To compile: CC buffer.cc -mt -lpthread
*/
#define _REEENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

#define BUFSIZE 64
#define NUM_THREADS 25

/* this is the data structure that is used between the producer
   and consumer threads */

struct {
   int buffer[BUFSIZE];
   pthread_mutex_t buflock;
   pthread_cond_t adddata;
   pthread_cond_t remdata;
   int nextadd, nextrem, occ, done;
} Buf;

/* Global data & lock */
pthread_mutex_t totlock;
int total;

/* function prototype */
extern "C" void *consumer(void*);

main()
{
   pthread_t cons_thr;
   int i, counter=0;
   pthread_attr_t attr;
   pthread_t threads[NUM_THREADS];

   /* initialise global data */
   Buf.nextadd = Buf.nextrem = Buf.occ = Buf.done = total = 0;

   /* initialize the mutex locks */
   pthread_mutex_init(&Buf.buflock, NULL);
   pthread_mutex_init(&totlock, NULL);

   /* set the contention scope to PTHREAD_SCOPE_SYSTEM */
   pthread_attr_init(&attr);
   pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);

   /* create the consumer threads */
   for (i=0;i<NUM_THREADS;i++)
      pthread_create(&threads[i], &attr, consumer, NULL);

   /* the producer ! */
   while (!Buf.done) {

      /* lock the buff mutex so consumer connot access it */
      /* (This blocks if buff mutex is already locked by a consumer) */
      pthread_mutex_lock(&Buf.buflock);

      /* check to see if buffer is full */
      /* If yes then wait for that condition to become false */
      while (Buf.occ == BUFSIZE)
         pthread_cond_wait(&Buf.remdata, &Buf.buflock);

      /* put random int into buffer */
      Buf.buffer[Buf.nextadd] = rand()%100;
      counter++;
      /* check to see if producing finished */
      if (counter == 1000)
         Buf.done = 1;

      /* inc buffer's input index */
      Buf.nextadd = ++Buf.nextadd % BUFSIZE;

      /* increment the number of items in buffer */
      Buf.occ++;

      /* signal the consumer - buff not empty incase it is waiting */
      pthread_cond_signal(&Buf.adddata);

      /* release the buff mutex */
      pthread_mutex_unlock(&Buf.buflock);

   }/* while */

   /* wait for the consumers to finish */
   for (i=0;i<NUM_THREADS;i++)
	   pthread_join(threads[i], NULL);

   /* print final total */
   printf("\b=%d\n",total);

   /* free the mutexes */
   pthread_mutex_destroy(&Buf.buflock);
   pthread_mutex_destroy(&totlock);

   /* free thread attribute */
   pthread_attr_destroy(&attr);

   /* exit the program */
   return(0);
}


/* The consumer thread */
extern "C" void *consumer(void *arg)
{
   int item;
   /* check to see if data in buffer or if the done flag is set */
   while (1) {

      /* lock the mutex so producer or other consumer cannot access it */
      /* (This blocks if buff mutex is already locked) */
      pthread_mutex_lock(&Buf.buflock);

      /* check to see if buffer is empty */
      /* if so, wait for data to be added */
      while (Buf.occ == 0 && !Buf.done)
         pthread_cond_wait(&Buf.adddata, &Buf.buflock);

      if (Buf.occ==0 && Buf.done){ /* no more data */
         //signal other consumers that may be waiting
    	pthread_cond_signal(&Buf.adddata);
         pthread_mutex_unlock(&Buf.buflock);
         break;
      }

      /* remove data item from the buffer */
      item = Buf.buffer[Buf.nextrem];

      /* set the next buffer to write from */
      Buf.nextrem = ++Buf.nextrem % BUFSIZE;

      /* decrement the number of items in buffer */
      Buf.occ--;

      /* signal the producer that buffer is not full */
      pthread_cond_signal(&Buf.remdata);

      /* release the buff mutex */
      pthread_mutex_unlock(&Buf.buflock);


      /* process the data */
      item %= 100;
      pthread_mutex_lock(&totlock);
      printf("%d+",item);
      total+=item;
      pthread_mutex_unlock(&totlock);

   }

   /* exit the thread */
   pthread_exit((void *)0);
   return(0);
}


