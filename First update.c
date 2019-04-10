#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>


sem_t barber,cust,Bchair;
static int chairs=0,cut_time=0,c_time=0,c_num=0;

//void func_Haircut();
void *func_Customers();
void *func_Barber_chair();

int main()
{	

	 //the attributes for program
	
	//pthread_t Haircut; // haircutting time or sleep
	pthread_t Customers; // updating incoming and outgoing customers
	pthread_t Barber_chair; //updating barber chair
	
	printf("\nEnter the number of chairs in customer waiting room\n");
	scanf("%d",&chairs); //initializing number of chairs

	printf("\nEnter average haircut time\n");
	scanf("%d",&cut_time);
	
	printf("\nEnter customer arrival gap\n");
	scanf("%d",&c_time);
	
	printf("\nEnter number of customers\n");
	scanf("%d",&c_num);
	
	printf("%d %d %d %d",chairs,cut_time,c_time,c_num);
	
	
	sem_init(&barber,0,0);
	sem_init(&Bchair,0,1);
	sem_init(&cust,0,0);

	//pthread_create (&Haircut, NULL, func_Haircut, NULL);
	pthread_create (&Barber_chair, NULL, func_Barber_chair, NULL); // creating the threads with respective functions
	pthread_join(Barber_chair,NULL);

	pthread_create (&Customers, NULL, func_Customers, NULL);	
	pthread_join(Customers,NULL);
	//sleep(c_time*c_num);
}

void *func_Barber_chair()
{

	int i=0;
	while (1)
	{	
		sem_wait(&cust); // waiting for client and blocks the code until value is further increadsed
		sem_wait(&Bchair); // taking first client
		if(c_num==0)
		{
			printf("\nthe barber has now 0 customers\nso he is sleeping\n");
			sem_post(&cust); // unlocking customer chairs
			sem_post(&Bchair); //unlocking barber chair
		}
		else
		{
			printf("\nCustomer %d gets on the Barber chair\n",++i);
			printf("\nWaiting chairs available are -->> %d\n",chairs);
			sem_post(&cust); // unlocking customer chairs
			sem_post(&Bchair); //unlocking barber chair
			printf("\nBarber cuts hairs for %d seconds\n" ,cut_time);
			sleep(cut_time); //sleep till haircut 
			printf("\nHAIRCUT FINISH");
			
			if (c_num==0)
			{
				printf("\nClients waiting are 0\nBarber goes to SLEEP\n");
			}
			
			else
			{
				printf("\nNext client comes to BARBER CHAIR\n");
			}
		}
			
	}
}

void *func_Customers()
{
	int i = 0;
	
	while(i!=c_num)
	{
		sleep(c_time);
		sem_wait(&cust);
		printf("\nNew Customer Enters\n");

		if(chairs==0)
		{
			printf("\nNo Seats Available\nCUSTOMER IS LEAVING WITHOUT HAIRCUT\n");
			sem_post(&cust);
		}

		else 
		{
			chairs--;
			printf("\nCustomer gets on WAITING ROOM SEAT\nSeats remaining--> %d\n",chairs);
			sem_post(&cust);
		}
		i++;
	}
}



 
