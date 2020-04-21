#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

 #define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1
 

static int
GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int
GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
static int
GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
 int
GPIORead(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
static int
GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		printf("error number:  %d",fd );
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////		MAIN     //////////////////////////////////

 
 // structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
 } message;
int main(int argc, char *argv[])
{
	
	
	int PINY[8]={17,18,27,22,23,24,25,4};    //Tablica adresow BCM pinow {GPIO_0,GPIO_1,GPIO_2,GPIO_3,GPIO_4,GPIO_5,GPIO_6,GPIO_7}
	bool stan;
	
	int i;
	int  msgid_zapytanie;
    int  msgid_odpowiedz;
	
	/////INICJALIZACJA/////
//stworzenie kolejki zapytania(dane od hello>read_write) i odpowiedzi(od read_write>hello)
	 msgid_zapytanie = msgget(1234, 0666 | IPC_CREAT);
	 msgid_odpowiedz = msgget(4321, 0666 | IPC_CREAT);
	  message.mesg_type = 1;
	
	  for(int i=0;i<8;++i){
				GPIOExport(PINY[i]);
					}
	
		while(1){
			printf("waiting for dataa\r\n");
			
			//msgrcv to receive message	
    msgrcv(msgid_zapytanie, &message, sizeof(message), 1, 0);


			// display the message
   printf("Data Received is : %s \n",  message.mesg_text);
			
		//zrobic zeby ktorykoliek pin mogl byc ?
			if(*(message.mesg_text)=='?'){

			
	for(i=0;i<8;++i){
	GPIODirection(PINY[i], IN);		
	printf("I'm reading %d on GPIO %d \n", GPIORead(PINY[i]),i);
	 
	 message.mesg_text[i]=GPIORead(PINY[i]);
	
					}
					
				 msgsnd(msgid_odpowiedz, &message, sizeof(message), 0);		
	
			}else
				{
								
	//WRITING BYTE TO PINS 
	for(i=0;i<8;++i){
		
			if(message.mesg_text[i]==48){
				stan=LOW;
				}else if(message.mesg_text[i]==49){
					stan=HIGH;
					}else{
sprintf(message.mesg_text, "zle dane" );
							}
		
	GPIODirection(PINY[i], OUT);
	GPIOWrite(PINY[i],stan);	
	printf("I wrote %d to PIN_%d \n", stan,i);
				}
		//	sprintf(message.mesg_text, "ok" );
	 
	 
	// msgsnd(msgid_odpowiedz, &message, sizeof(message), 0);
			
		}
	}
		
		


 
	return(0);
}


                    
 
    

