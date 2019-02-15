/*
Homework 2
CS-531 Fall 2016

Group Members: Randive Onkar Mahadev
		G#-G01036553
*/

#include<stdio.h>										//Standard Input/Output
#include<stdlib.h>										//General Utilities
#include<signal.h>										//Signal handling
#include<fcntl.h>
#include<unistd.h>									
#include<sys/stat.h>
#include<sys/wait.h>

int	cs531_system(char *s);								//Declaration of user defined system function cs531_system
void sig_handler(int signo);							//Declaration of user defined function used for signal handling 
int count ;												//Global variable to store the number of arguments passed i.e argc
char * name;											//Global variable to store argv[0]
int check;												//Global variable to save SIGINT and check SIGINT
	
int	main(int argc,	char *argv[])	
{	
		int	status;	
		count=argc;
		name= argv[0];
		status= cs531_system(argv[1]);					//Function Call
		sleep(6);										
		return	status;	
}	

int cs531_system(char *s)
{
	char * buffr;
	buffr=s;
	int c,trmnl;
	pid_t child,cstatus;
	
	trmnl=open("/dev/tty",O_RDWR);						//Terminal access
	close(0);
	dup(trmnl);
	close(1);		
	dup(trmnl);
	close(2);
	dup(trmnl);	
	close(trmnl);
	if(count<2)											//Check if command entered
	{	fprintf(stderr,"No Command\nPlease enter a valid command%s",name);			//No command entered
		exit(1);
	}
	
	child=fork();										//child process created
	
	if (child==(-1))									//check if child is created properly
	{	perror("fork error");
		exit(1);
	}
	else if(child==0)									//child process
	{
		execlp("/bin/sh","/bin/sh","-c",buffr,(char*)NULL);			//"/bin/sh" is the path passed to execlp, buffr is argv[] containing 
		fprintf(stderr,"execlp failed");							//command to be executed//////execlp arguments are terminated by a NULL pointer
		exit(1);
	}
	else
	{																// Parent process
		
		if (signal(SIGINT,sig_handler)==SIG_ERR)					//call to signal handler
		{
			printf("\n	can't	catch	SIGINT	\n");	
		}
		while(check==SIGINT)
		{
			//sleep(10);
			exit(1);
		}
		c = wait(&cstatus);											//wait for child process to terminate
		return(c);
	}
	
}
void sig_handler(int signo)	
{	
	if(signo==SIGINT)
	{
		check=signo;
		//printf("received SIGINT");
	}						
}