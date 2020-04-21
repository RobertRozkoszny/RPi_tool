#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>
#include "json/json.h"
#include "json/json-forwards.h"
#include <stdlib.h>

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
    } message;
using namespace std;

int main()
{
	
cout << "Content-Type: text/plain\r\n\r\n" << endl << endl ;	
	 
	// structure for message queue

	int  msgid_zapytanie;
    int  msgid_odpowiedz;
  //inicjalizacja- podlacznie do kolejek
   msgid_zapytanie = msgget(1234, 0666);
   msgid_odpowiedz = msgget(4321, 0666);

    message.mesg_type = 1;

	char *query;
	query= getenv( "QUERY_STRING" );  
	int i=0;
  
 while(*query!=0){
	 
	 if(*query==61){
		 message.mesg_text[i]=*(query+1);

		 if(*(query+1)=='%' & *(query+2)=='3' & *(query+3)=='F')  message.mesg_text[i]='?';
		  i++;
		  }
	 query++;
	 }
 
    // msgsnd to send message
    msgsnd(msgid_zapytanie, &message, sizeof(message), 0);
 
    // display the message

 
 Json::Value fromScratch;
while((msgrcv(msgid_odpowiedz, &message, sizeof(message), 1,IPC_NOWAIT ))!=(-1)){

    char PIN[3];
	int stan;
	
for(i=0;i<8;i++){
	sprintf(PIN, "P%d",i);
    fromScratch[PIN] = int(message.mesg_text[i]);
}	
}
Json::StyledWriter styledWriter;
std::cout << styledWriter.write(fromScratch);

    return 0;
}




	 
 
   
	
