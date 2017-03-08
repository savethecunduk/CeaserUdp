#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 256

void append(char* arr,char c){
	int len = strlen(arr);
	arr[len] = c;
	arr[len+1] = '\0';
}



int main (int argc, char* argv[]){
	int clientSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(clientSocket<0){
		printf("Unable to create the Socket to send messages\n");
		return 0;
	}
	//Configure socket address
	struct sockaddr_in peer;
	char destIP[100] = "127.0.0.1";
	int peerAddrSize = sizeof(peer);
	peer.sin_family = AF_INET;
	peer.sin_addr.s_addr = inet_addr(destIP);
	peer.sin_port = htons(20000);
	printf("Connection Established!\n");
	//Print CeaserCipher
	printf("This application send CeaserCipher Encrypted Message to Server over UDP Protocol\n");
	printf("\n");
	printf("================================\n");
	
	//Alphabet
	const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
	//const char *plainText = "hellomynameiscem";
	while(1){
		char plainText[BUFFER_SIZE];
		printf("Enter To Message:");
		scanf("%s",plainText);
		char *plainPointer = (char *)&plainText;
		int len = strlen(plainText);
		int count = 0;
		char cipherText[BUFFER_SIZE]={0};
		int key = 3;
		while(count<len){
			char *p = strchr(alphabet,*(plainPointer + count));
			int position = (int) (p-alphabet);
			position = position + key;
			int temp = position %26;
			char c1 = *(alphabet+temp);
			append(cipherText,c1);
			count++;
		}//end of while
		int ret = sendto(clientSocket,cipherText,BUFFER_SIZE,0,(struct sockaddr *)&peer,peerAddrSize);
		if(ret<0){
			printf("Message Cannot Sended\n");
			return 0;
		}
	}
	close(clientSocket);
	return 0;
}//End of Main
