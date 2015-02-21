/*
 * @author : James Park, Ryan Smith, Rajveer Parikh
 */
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

char string[1000];
char ready_string[1000];
int kill_threads;
int fd;
pthread_mutex_t lock;
pthread_mutex_t lock2;

//thread for reading time data from arduino
void* fun1(void* arg){
//read some bytes
	while(1){
	  pthread_mutex_lock(&lock);
	  if(kill_threads == 0){ 
	    pthread_mutex_unlock(&lock);
	    break;
	  }
	  pthread_mutex_unlock(&lock);
	  char buf[100];
	  int bytes_read = read(fd, buf, 100);
	  int i;
	  for(i = 0; i < bytes_read; i++){
	  	if (buf[i] == '\n'){
			char null = '\0';
			//appends null character to complete working string
			strcat(string, &null);
			//clears ready string
			pthread_mutex_lock(&lock2);
			ready_string[0] = null;
			//appends completed working string to ready string
			strcat(ready_string, string);
			pthread_mutex_unlock(&lock2);
			//clear working string to begin again
			string[0] = null;
			continue;	
		}
		//simply adds individual character to working string
		char this_char = buf[i];
		strncat(string, &this_char, 1);	
	  }
	}
	return NULL;
}

//thread to request and process user input
void* fun2(void* arg){
	char* on = "o";
	char* off = "f";
	while(kill_threads == 1){
		char input[10];
		printf("Enter user input: ");
		scanf("%s", input);
		switch (input[0]){
			case('a'):
				write(fd, on, strlen(on));
				break;
			case('b'):
				write(fd, off, strlen(off));
				break;
			case('q'):
				pthread_mutex_lock(&lock);
				kill_threads = 0;
				pthread_mutex_unlock(&lock);
				break;
			default:
				pthread_mutex_lock(&lock2);
				printf("%s\n", ready_string);
				pthread_mutex_unlock(&lock2);
		}
	}
	return NULL;
}


int main(){
  
	kill_threads = 1;
	pthread_t t1, t2;
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&lock2, NULL);

	//establish connection w/device file("open file")
	fd = open("/dev/ttyUSB11", O_RDWR);
	if(fd == -1){// couldn't open
	  printf("Couldn't open\n");
	  return 0;
	}

	//configure connection
	struct termios options;

	tcgetattr(fd, &options);
	cfsetispeed(&options, 9600); //how fast to receive
	cfsetospeed(&options, 9600); //how fast to send

	tcsetattr(fd, TCSANOW, &options);

	//create threads and attach to functions
	pthread_create(&t1, NULL, &fun1, NULL);
	pthread_create(&t2, NULL, &fun2, NULL);

	//join threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	printf("Threads are dead!\n");
	
	//close
	close(fd);
	return 1;
}
