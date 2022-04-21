#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h> 
#include <fcntl.h>
#include <syslog.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdbool.h>
#include <inttypes.h>

#include "logfile.c"

void ctrlc_handler(int sig)
{
	// printf("CTRL-C pressed!\n");
	// TODO: Socket schließen etc....
	system("killall mouse_shared &");
	exit(0);
}

void sigpipe_handler(int unused)
{
	// Broken Pipe
	logfile("Broken pipe: Client disconnected!");
	logfile("Terminating child thread now...");
	_exit(1);
}

#include "write_history.c"

int write_to_memory(char* str)
{ 
    // ftok to generate unique key 
    key_t key = ftok("/",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *_str = (char*) shmat(shmid,(void*)0,0); 
  
    //printf("Write Data : ");
	strcpy(_str, str);
	//puts(str);
  
    //printf("Data written in memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 
	logfile("Successfull write_to_memory()");
    return 0;
}

// if "everysecond" is set, fakepress the bell every second
int read_from_memory(char *sendBuff, int newsockfd, char *argv[], int argc)
{
    // ftok to generate unique key 
    key_t key = ftok("/",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 

	while(1) {
		if(argc >= 2) {
			if(strcmp(argv[1],"-fakebell") == 0) {
				// Test if client alive
				snprintf((char *)sendBuff, sizeof((char *)sendBuff)+1, "%s", "I");
				if(write(newsockfd, (char *)sendBuff, strlen((char *)sendBuff)) == -1) {
					logfile("Inside memory-function (fakebell): client_broken or closed!");
					exit(1);
				}
				
				time_t rawtime;
				struct tm * timeinfo;
				char secs[3] = {0};
				int secs_ = 0;
				bool everysecond = NULL;
				
				if(argc == 3) {
					if(strcmp(argv[2],"1") == 0)
						everysecond = true;
					else
						everysecond = false;
				}

				while(1) {
				    time (&rawtime);
					timeinfo = localtime (&rawtime);
					secs[0] = asctime(timeinfo)[17];
					secs[1] = asctime(timeinfo)[18];
					secs[2] = '\0';
					secs_ = strtoimax(secs,NULL,10);
					
					if(everysecond) goto jump1;
					
					//if(strcmp(secs, "30") == 0 || strcmp(secs, "00") == 0) {
					if(secs_ == 0 || secs_ == 30) {
						jump1:
						logfile("FAKEBELL");
						snprintf((char *)sendBuff, sizeof((char *)sendBuff)+1, "%s", "BELL");
						if(write(newsockfd, (char *)sendBuff, strlen((char *)sendBuff)) == -1)
						{
							logfile("Inside memory-function: client_broken or closed!");
							exit(1);
						}
						write_to_history_file();
						sleep(everysecond ? 1 : 2);
						break;
					}
					else {
						// Wait (Very importnat!)
						usleep(20 * 1000);
					}
				}
			}
		}
		else {
			// If bell was pressed
			if(strcmp(str, "BELL") == 0) {
				logfile("BELL");
				snprintf((char *)sendBuff, sizeof((char *)sendBuff)+1, "%s", "BELL");
				if(write(newsockfd, (char *)sendBuff, strlen((char *)sendBuff)) == -1) {
					logfile("Inside memory-function: client_broken or closed!");
					exit(1);
				}
				break;
			}
			else {
				// bell not pressed this time, test if client is alive and exit child if not
				snprintf((char *)sendBuff, sizeof((char *)sendBuff)+1, "%s", "I");
				if(write(newsockfd, (char *)sendBuff, strlen((char *)sendBuff)) == -1) {
					logfile("Inside memory-function: client_broken or closed!");
					exit(1);
				}
				usleep(20 * 1000);
			}
		}
	}

    // detach from shared memory  
    shmdt(str); 
    
    // destroy the shared memory 
    // shmctl(shmid,IPC_RMID,NULL);
	
	// reset
	write_to_memory("CLEARED");
	
    return 0;
}

int main(int argc, char *argv[])
{
    int listenfd = 0, newsockfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
    //char testIfBroken[1];
    time_t ticks; 
	int read_size;
	int fd, bytes;
    unsigned char data[3];
    const char *pDevice = "/dev/input/mice";
	char client_message[2000];
	int counter;
	int pid; // Handle multiple clients
	//struct timeval timeout;
	//struct timeval timeout;
	
	logfile("Started");
	setvbuf(stdout, NULL, _IONBF, 0);
    printf("Bell-Server v1.0\nUsage: %s", argv[0]);
	printf(" [-fakebell]");
	printf("\n\nIf -fakebell is set, server sends bell-signals every 30 seconds (e.g. for delay testing-purposes).\n");
	printf("Use '-fakebell 1' for every 1 second.\n\n");

	// execute mouse-checking and writing BELL-state to shared memory
	system("./mouse_shared &");
	
	sigaction(SIGPIPE, &(struct sigaction){sigpipe_handler}, NULL);

	// CTRL + C: end mouse_shared program
	signal(SIGINT, ctrlc_handler);
	
    //timeout.tv_sec = 86400;  // = 24h;
    //timeout.tv_usec = 0;
	
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff)); 
	//memset(testIfBroken, '0', sizeof(testIfBroken)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5193); 

	/*if (setsockopt (newsockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
		sizeof(timeout)) < 0)
        logfile("setsockopt_snd failed\n");
	if (setsockopt (newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
		sizeof(timeout)) < 0)
        logfile("setsockopt_rcvt failed\n");*/

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
			
    listen(listenfd, 10);
    while(1)
    {
		newsockfd = accept(listenfd, (struct sockaddr*)NULL, NULL);	
		if (newsockfd < 0) {
			logfile("ERROR on accept!");
			return -1;
		}
		//fork new process
        pid = fork();
        if (pid < 0) {
			logfile("ERROR in new process creation");
        }
        if (pid == 0) {
			//child process
			close(listenfd);
			//do whatever you want
			////////////////////

			int n = 0;
			char buffer[256];
			bzero(buffer,256);
			int client_bytes = 0;
			
			if((client_bytes = read(newsockfd,buffer,9)) > 0) {
				// Before sending, test if client request was "want_bell"
				if(strcmp(buffer, "want_bell") == 0) {  // Want to have bell-signals
					bzero(buffer,256);
					// reset, because memory could have the last door-pressed bell
					write_to_memory("CLEARED");

					while(1) {
						//if(write(newsockfd, testIfBroken, strlen(testIfBroken)) == -1) {
						//	logfile("client_broken or closed!");
						//	break;
						//}
						read_from_memory(sendBuff,newsockfd,argv,argc);
						//close(newsockfd);
						//newsockfd = -1;
						//break;
						// End child process
						//kill(pid, SIGTERM);
					}
				}
				else {
					logfile("want_bell_NOT_there");
				}
			}
			else if(client_bytes == 0) {
				logfile("Client disconnected");
				break;
			}
			else {
				logfile("Client read failed");
			}
			////////////////////
            close(newsockfd);
        } else {
			//parent process
			close(newsockfd);
        }
    }
}