#include<stdio.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h> 
using namespace std;

int main() {
	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = inet_addr("127.0.0.1");
	

	if(connect(client_socket, (struct sockaddr*)&client, sizeof(client)) == -1) {
        printf("Connection failed");
        return 0;
	}
	char msg1[50]="ACK-0";
    char msg2[50]="ACK-1";

	char buff[100];
	int flag=1,flg=1,flag2=1;

	for(int i=0;i<10;i++) {
		flg=1;
		bzero(buff,sizeof(buff));
        if((i==3&&flag==1)||(i==7&&flag2==1)){
			printf("\nSimulating a corruption/problem...\n");
			if(i==3&&flag==1)
			    flag=0;
			else if(flag==0)
			    flag2=0;            
			i--;
			read(client_socket,buff,sizeof(buff));

			continue;
        }
		int n = read(client_socket, buff, sizeof(buff));

        if(buff[strlen(buff)-1]!=i+'0'){          //ACK not received at server,so discard
        printf("\nDiscarding packet...\n");

		i--;
		continue;

        }
		printf("Message received from receiver : %s \n",buff);
        printf("ACK sent \n");
        qq:
        if(i%2==0)
		    write(client_socket,msg2, sizeof(msg2));
        else 
 		    write(client_socket,msg1, sizeof(msg1));    
		if(flg==0)
		    i--;   
	}
	
	close(client_socket);
	return 0;
}