#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

	
	
void *agent_process();
void *smoker_with_match(int i);
void *smoker_with_tobacco(int i);
void *smoker_with_paper(int i);
void randwait(int);
int no_smokers,no_agents;
int execute=1;
int no_seconds;

sem_t agent;
sem_t paper_tobacco;
sem_t match_paper;
sem_t tobacco_match;
sem_t lock;

int tobacco_smokers=0, paper_smokers=0, match_smokers=0;

int main()
{
	int i,j,k,no_tobacco=0,no_paper=0,no_match=0,n;
	
	printf("Enter the number of Agents-->");
	scanf("%d",&no_agents);
	printf("\nEnter the no. of SMOKERS = ");
	scanf("%d",&no_smokers);
	printf("\n How long (in seconds) do u want the program to execute ? (press 0 for infinite execution)-->");
	scanf("%d", &no_seconds);
	//n=no_smokers/3;
	sem_init(&agent, 0, 0);
	sem_init(&paper_tobacco, 0,0);
	sem_init(&match_paper, 0,0);
	sem_init(&tobacco_match,0,0);
	sem_init(&lock,0,1);
	//pthread_t  agent,p_smoker[no_smokers],t_smoker[no_smokers],m_smoker[no_smokers];
	pthread_t *agent, *p_smoker, *t_smoker, *m_smoker;
	
	agent=(pthread_t *)malloc(no_agents*(sizeof(pthread_t)));
	p_smoker=(pthread_t *)malloc(no_smokers*(sizeof(pthread_t)));
	t_smoker=(pthread_t *)malloc(no_smokers*(sizeof(pthread_t)));
	m_smoker=(pthread_t *)malloc(no_smokers*(sizeof(pthread_t)));
	
	for(i=0;i<no_agents;i++)
		pthread_create(&agent[i],NULL,(void *)agent_process,(void*)i);
	for(i=0;i<no_smokers;i++)
	{
		j=random();
		if(j%3==0)
		{
			tobacco_smokers++;
			k=tobacco_smokers;
			pthread_create(&t_smoker[i],NULL,(void *)smoker_with_tobacco,(void *)k);
			

		}
		else if(j%3==2)
		{
			match_smokers++;
			k=match_smokers;
			pthread_create(&m_smoker[i],NULL,(void *)smoker_with_match,(void *)k);
			
		}
		else
		{
			paper_smokers++;
			k=paper_smokers;
			pthread_create(&p_smoker[i],NULL,(void *)smoker_with_paper,(void *)k);
			
		}
	}
	/*for(i=0;i<15;i++)
	{
		if(i<5)
		{
			pthread_create(&t_smoker,NULL,(void *)smoker_with_tobacco,(void *)i);
		}
		else if(i>=4&&i<10)
		{
			pthread_create(&p_smoker,NULL,(void *)smoker_with_paper,(void *)i);
		}
		else
		{
			pthread_create(&m_smoker,NULL,(void *)smoker_with_match,(void *)i);
		}
	}*/
	
	while(no_seconds==0);	
	sleep(no_seconds);
	execute=0;
	for(i=0;i<no_agents;i++)
	pthread_join(agent[i],NULL);
	/*for(i=0;i<no_smokers;i++)
	{
		if(i%2==0)
		{
			pthread_join(t_smoker[i],NULL);

		}
		else if(i%3==0)
		{
			pthread_join(m_smoker[i],NULL);
		}
		else
		{
			pthread_join(p_smoker[i],NULL);
		}
	}*/
	printf("\n\n");
	return 0;
}

void *agent_process(int i)
{
	int count=no_smokers,k=1;
	while(execute)
	{
		sem_wait(&lock);		//lock to check no other process enters
		//int j=rand()%10000;
		int j=random();
		if(j%3==0)
		{
			printf("\n******Paper and Match is produced by agent %d******",i);
			sem_post(&match_paper);
		}
		else if(j%3==1)
		{
			printf("\n******Paper and Tobacco is produced by agent %d******",i);
			sem_post(&paper_tobacco);
		}
		else
		{
			printf("\n******Tobacco and Match is produced by agent %d******",i);
			sem_post(&tobacco_match);
		}
		//sem_post(&lock);
		sem_wait(&agent);		//agent goes to sleep
		//count--;
	}
	pthread_exit(0);
}

void *smoker_with_paper(int i)
{
	printf("\nSmoker %d with only paper produced",i); 
	while(1)
	{	
		int k;
		sem_wait(&tobacco_match);
		printf("\nSmoker %d with only paper joining",i); //sem_wait(&lock);
		printf("\n The smoker %d with only paper is SMOKING :-* \n",i);
		//for(k=0;k<100;k++);
		randwait(1);
		sem_post(&agent);	
		sem_post(&lock);
		//printf("\n\n");
	}
	pthread_exit(0);
}

void *smoker_with_tobacco(int i)
{
	printf("\nSmoker %d with only tobacco produced",i); 
	while(1)
	{
		int k;
		sem_wait(&match_paper);
		printf("\nSmoker %d with only tobacco joining",i); //sem_wait(&lock);
		printf("\n The smoker %d with only tobacco is SMOKING :-* \n",i);
		//for(k=0;k<100;k++);
		randwait(1);
		sem_post(&agent);	
		sem_post(&lock);
		//printf("\n\n");
	}
	pthread_exit(0);
}

void *smoker_with_match(int i)
{
	printf("\nSmoker %d with only match produced",i); 
	while(1)
	{
		int k;
		sem_wait(&paper_tobacco);
		printf("\nSmoker %d with only match joining",i); //sem_wait(&lock);
		printf("\n The smoker %d with only match is SMOKING :-* \n",i);
		//for(k=0;k<100;k++);
		randwait(1);
		sem_post(&agent);	
		sem_post(&lock);
		//printf("\n\n");
	}
	pthread_exit(0);
}

void randwait(int secs)
{ 
	int len; // Generate a random number... 
	len = (int) ((drand48() * secs) + 0.3); 
	sleep(len); 
} 
