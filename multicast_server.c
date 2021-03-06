/* Send Multicast Datagram code example. */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000000

struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;
char databuf[BUFFER_SIZE] = "Multicast test message lol!";
int datalen = sizeof(databuf);
 
int main (int argc, char *argv[ ])
{
/* Create a datagram socket on which to send. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
	else
	  printf("Opening the datagram socket...OK.\n");
	 
	/* Initialize the group sockaddr structure with a */
	/* group address of 226.1.1.1 and port 4321. */
	memset((char *) &groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(4321);
	 
	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */
	localInterface.s_addr = inet_addr("10.0.2.15");
	
	/* IP_MULTICAST_IF:  Sets the interface over which outgoing multicast datagrams are sent. */
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	  perror("Setting local interface error");
	  exit(1);
	}
	else
	  printf("Setting the local interface...OK\n");
	
	//Read the file
	FILE *fp = fopen(argv[1],"r");
	if(fp == NULL){
		printf("Error!Could not open file\n");
		exit(-1);
	}
    
	//Calculate the file size
	fseek(fp,0L,SEEK_END);
	int flen = ftell(fp);
	fseek(fp,0L,SEEK_SET);
	
	//Read and Send the file
	bzero(databuf,BUFFER_SIZE);
	int buffer_no=0;
	while((datalen=fread(databuf,sizeof(char),BUFFER_SIZE,fp))>0){
	/* Send a message to the multicast group specified by the*/
	/* groupSock sockaddr structure. */
	/*int datalen = 1024;*/
		if(sendto(sd, databuf, datalen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
		{
			perror("Sending datagram message error");
		}
		else
	  	printf("Sending datagram message %d...OK\n",++buffer_no);
		bzero(databuf,BUFFER_SIZE);
	}
	
	//To mention stop transfer, send a buffer with size 0
	sendto(sd, databuf, 0, 0, (struct sockaddr*)&groupSock, sizeof(groupSock));

	printf("FILE SIZE = %.4lfKB\n",(double)flen/1000);
	/* Try the re-read from the socket if the loopback is not disable
	if(read(sd, databuf, datalen) < 0)
	{
	perror("Reading datagram message error\n");
	close(sd);
	exit(1);
	}
	else
	{
	printf("Reading datagram message from client...OK\n");
	printf("The message is: %s\n", databuf);
	}
	*/
	return 0;
}
