#include<stdio.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h> 
#include<fcntl.h>
using namespace std;

int main() {
	int server_socket, client_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server, client;
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	socklen_t add;
	

	bind(server_socket, (struct sockaddr*)&server, sizeof(server));

	printf("\tServer (Sender) starting...\n Stop and wait protocol used to send 10 messages \n\n");
    cout<<"It helps flow control by::"<<endl<<"1.As it ensures that the message delivered by the sender receives the recepient hence a flow control is maintained"<<endl;

	cout<<"**IN OUR PROGRAM ,I'M SENDING 10 MESSAGES;AND IMPLEMENTING SOMETHING SIMILAR TO RDT -3.0  **"<<endl<<"	1.IN THE THIRD MESSAGE THE ACK IS LOST SO A RETRANSMISSION OCCRUS "<<endl<<"	2.INSTEAD OF SENDING AN INTEGER IN THE 6TH MESSAGE(HERE WORKING OF CHECKSUM IS MIMICKED BY CHECKING WHETHER THE MESSAGE IS AN INT OR NOT) I SENT A \"?\" "<<endl;
	cout<<"	3.AND TO SIMULATE DELAYED ACK,I'M FORCING A TIMEOUT AT THE 7TH MESSAGE"<<endl<<"**THUS ALL 3 CASES OF CONCERN ARE TAKEN CARE HERE**"<<endl<<endl;
    
	listen(server_socket, 10); 

	add = sizeof(client);
	client_socket = accept(server_socket, (struct sockaddr*)&client, &add);
    time_t t1,t2;
	char msg[50]="Server(sender) message : ";
	char buff[50];
    int flag=0;

    fd_set set;
    struct timeval timeout;
    int rv;




	for(int i=0;i<10;i++) {
		if(i==6&&flag==0){
            i=15;
            printf("\nSending a corrupt packet(simulating..)...\n");		//for simulating corruption
		}
        bzero(buff,sizeof(buff));
		char buff2[60];
		qq:
		bzero(buff2,sizeof(buff2));
		strcpy(buff2,msg);
		buff2[strlen(msg)]=i+'0';
		
		printf("Message sent to receiver :%s \n",buff2);
  
		FD_ZERO(&set); 
  		FD_SET(client_socket, &set); 

  		timeout.tv_sec = 2;
  		timeout.tv_usec = 0;
		write(client_socket, buff2, sizeof(buff2));
        rv = select(client_socket + 1, &set, NULL, NULL, &timeout);
        if(rv == -1)
            perror("select "); // an error accured 
        else if(rv == 0){
            printf("\nResending message ..\n");
                if(i==15)
                i=6;
                goto qq;
        } // a timeout occured 
        else
            read(client_socket, buff, sizeof(buff));
	    if(buff[strlen(buff)-1]!=(i%2==0)+'0'){
            printf("\nACK error... %s\n",buff);
            goto qq;
        }

		printf("Message from receiver: %s\n", buff);
	}
	
	close(client_socket);
	close(server_socket);
	return 0;
}