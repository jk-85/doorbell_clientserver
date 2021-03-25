#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h>
#include <time.h>

int write_to_memory(char* str)
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *_str = (char*) shmat(shmid,(void*)0,0); 
  
    //printf("Write Data : ");
	strcpy(_str, str);
	//puts(str);
  
    //printf("Data written in memory: %s\n",_str); 
      
    //detach from shared memory  
    shmdt(_str); 
  
    return 0;
}

#include "write_history.c"

int main(int argc, char** argv)
{
    int fd, bytes;
    unsigned char data[3];
    /*int *ptr = mmap ( NULL, 1*sizeof(int),
            PROT_WRITE,
            MAP_SHARED,
            0, 0 );*/
    	
    const char *pDevice = "/dev/input/mice";

    // Open Mouse
    fd = open(pDevice, O_RDWR);
    if(fd == -1)
    {
        printf("ERROR Opening %s\n", pDevice);
        return -1;
    }

    int left, middle, right;
    signed char x, y;
	write_to_memory("NOTHING");
	
    while(1)
    {
		// Read Mouse
		bytes = read(fd, data, sizeof(data));
		if(bytes > 0)
		{
			left = data[0] & 0x1;   // not used but cable connected
			right = data[0] & 0x2;    // connected!!
			middle = data[0] & 0x4; // not used but cable connected

			x = data[1];
			y = data[2];
			//printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
			//printf("right=%d\n", right);
			if(right == 2) {
				//printf("Bell pressed!!!!!!\n");
				write_to_memory("BELL");
				write_to_history_file();
			}
		}
		//usleep(50 * 1000);
    }
    return 0; 
}
