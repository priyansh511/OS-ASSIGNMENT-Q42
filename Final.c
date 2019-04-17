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
  
 
  printf("\nEnter the Number of Chairs : ");
  scanf("%d",&num_chairs);
  printf("\nEnter the number of clients : ");
  scanf("%d",&clients);
  printf("\nEnter the Haircut time of customers : ");
  scanf("%d",&runTime);
  printf("\nEnter Average client wait time : ");
  scanf("%d",&clientWait);
  
   sem_init(&chairs_mutex,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
  
   pthread_create(&barberid, NULL, barber, NULL);
   printf("\nCreating barber thread with id %lu\n",barberid);
  
   for (i = 0; i < clients; i++){
	   pthread_create(&clientids[i], NULL, client, NULL);
	   printf("\nCreating client thread with id %lu\n",clientids[i]);
   }
  
   printf("\nMain thread sleeping for %d seconds\n", runTime);
   sleep(runTime);
   printf("\nMain thread exiting\n");
   exit(0);
}

void *barber() {
  
   while(1) {

	  sem_wait(&sem_client);

	  sem_wait(&chairs_mutex);

	  num_chairs += 1;
	  printf("\nBarber: Taking a client. Number of chairs available = %d\n",num_chairs);

	  sem_post(&sem_barber);

	  sem_post(&chairs_mutex);

	  printf("Barber: Cutting hair for %d seconds\n\n\n", runTime);
	  sleep(runTime);
    } 
}

void *client() {

   while(1) {
	  sem_wait(&chairs_mutex);

	  if(num_chairs <= 0){

		   printf("\nClient: Thread %u leaving with no haircut\n", (unsigned int)pthread_self());
       sleep(1);
		   sem_post(&chairs_mutex);
	  }
	  else{

		   num_chairs -= 1;
		   printf("Client: Thread %u Sitting to wait. Number of chairs left = %d\n",(unsigned int)pthread_self(),num_chairs);
       if(num_chairs==0)
        num_chairs++;
       sleep(1);

		   sem_post(&sem_client);

		   sem_post(&chairs_mutex);

		   sem_wait(&sem_barber);

		   printf("Client: Thread %u getting a haircut\n",(unsigned int)pthread_self());
       sleep(1);
	  }

	  printf("Client: Thread %u waiting %d seconds before attempting next haircut\n\n\n",(unsigned int)pthread_self(),clientWait);
	  sleep(clientWait);
     }
}
