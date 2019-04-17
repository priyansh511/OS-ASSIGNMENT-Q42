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
   /* 3. Create barber thread. */
   pthread_create(&barberid, NULL, barber, NULL);
   printf("\nCreating barber thread with id %lu\n",barberid);
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

void *barber() {
   //int worktime;
  
   while(1) {
      /* wait for a client to become available (sem_client) */
	  sem_wait(&sem_client);
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&chairs_mutex);
      /* increment number of chairs available */
	  num_chairs += 1;
	  printf("\nBarber: Taking a client. Number of chairs available = %d\n",num_chairs);
      /* signal to client that barber is ready to cut their hair (sem_barber) */
	  sem_post(&sem_barber);
      /* give up lock on chair count */
	  sem_post(&chairs_mutex);
      /* generate random number, worktime, from 1-4 seconds for length of haircut.  */
	  //worktime = (rand() % 4) + 1;
      /* cut hair for worktime seconds (really just call sleep()) */
	  printf("Barber: Cutting hair for %d seconds\n\n\n", runTime);
	  sleep(runTime);
    } 
}
