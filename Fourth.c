//gcc -Wall -pthread 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *client();
void *barber();

sem_t chairs_mutex;
sem_t sem_client;
sem_t sem_barber;
int num_chairs;
int clientWait;
int runTime;
int clients;
int MAX=20;
int main() {
	pthread_t barberid;
	pthread_t clientids[MAX];
	int i;
  
  /*printf()
  printf("Main thread beginning\n");
    1. Get command line arguments 
   int runTime,clients,i;
   if (argc != 5){
	   printf("Please enter 4 arguments: <Program run time> <Number of clients>\n");
	   printf("<Number of chairs> <Client wait time>\n");
	   exit(0);
   }
   runTime = atoi(argv[1]);
   clients = atoi(argv[2]);
   num_chairs = atoi(argv[3]);
   clientWait = atoi(argv[4]);
  */
  printf("\nEnter the Number of Chairs : ");
  scanf("%d",&num_chairs);
  printf("\nEnter the number of clients : ");
  scanf("%d",&clients);
  printf("\nEnter the Haircut time of customers : ");
  scanf("%d",&runTime);
  printf("\nEnter Average client wait time : ");
  scanf("%d",&clientWait);
   /* 2. Initialize semaphores */
   sem_init(&chairs_mutex,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
 
 
   /* 4. Create client threads.  */
   for (i = 0; i < clients; i++){
	   pthread_create(&clientids[i], NULL, client, NULL);
	   printf("\nCreating client thread with id %lu\n",clientids[i]);
   }
   /* 5. Sleep. */
   printf("\nMain thread sleeping for %d seconds\n", runTime);
   sleep(runTime);
   /* 6. Exit.  */
   printf("\nMain thread exiting\n");
   exit(0);
}

void *client() {
   //int waittime;

   while(1) {
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&chairs_mutex);
      /* if there are no chairs */
	  if(num_chairs <= 0){
           /* free mutex lock on chair count */
		   printf("\nClient: Thread %u leaving with no haircut\n", (unsigned int)pthread_self());
       sleep(1);
		   sem_post(&chairs_mutex);
	  }
      /* else if there are chairs */
	  else{
           /* decrement number of chairs available */
		   num_chairs -= 1;
		   printf("Client: Thread %u Sitting to wait. Number of chairs left = %d\n",(unsigned int)pthread_self(),num_chairs);
       if(num_chairs==0)
        num_chairs++;
       sleep(1);
           /* signal that a customer is ready (sem_client) */
		   sem_post(&sem_client);
           /* free mutex lock on chair count */
		   sem_post(&chairs_mutex);
           /* wait for barber (sem_barber) */
		   sem_wait(&sem_barber);
           /* get haircut */
		   printf("Client: Thread %u getting a haircut\n",(unsigned int)pthread_self());
       sleep(1);
	  }
      /* generate random number, waittime, for length of wait until next haircut or next try.  Max value from command line. */
	 // waittime = (rand() % clientWait) + 1;
      /* sleep for waittime seconds */
	  printf("Client: Thread %u waiting %d seconds before attempting next haircut\n\n\n",(unsigned int)pthread_self(),clientWait);
	  sleep(clientWait);
     }
}
