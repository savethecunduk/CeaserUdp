#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 256
void DecryptMessages(char* receivedData);
void append(char* arr,char c);
void DecryptMessages(char* receivedData){
	char *p = (char *)receivedData;
	const char *alphabet  = "abcdefghijklmnopqrstuvwxyz";
	int len = strlen(receivedData);
	int count = 0;
	int key = 3;
	char plainText[BUFFER_SIZE] = {0};
	while(count<len){
		char *run =  strchr(alphabet,*(p+count));
		int position = (int)(run-alphabet);
		position = position - key;
		int temp = position %26;
		char s = *(alphabet + temp);
		append(plainText,s);
		count++;
	}// end of while
	printf("Message is:%s\n",plainText);
}
void append(char* arr,char c){
	int l = strlen(arr);
	arr[l] = c;
	arr[l+1] = '\0';
}
int main(int argc,char* argv[]){
	int serverSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(serverSocket<0){
		printf("An Error Occured when creating Server Socket\n");
		return 0;
	}
	struct sockaddr_in me;
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = htonl(INADDR_ANY);
	me.sin_port = htons(20000);
	//Binding
	int ret = bind(serverSocket,(struct sockaddr *)&me,sizeof(me));
	if(ret<0){
		printf("An error occured when binding\n");
		return 0;
	}
	//const char *alphabet  = "abcdefghijklmnopqrstuvwxyz"; 
	printf("Receiving Encrypted Messages......\n");
	while(1){
	struct sockaddr_in peer;
	int peerAddrSize = sizeof(peer);
	char receivedData[BUFFER_SIZE];
	ret = recvfrom(serverSocket,receivedData,BUFFER_SIZE,0,(struct sockaddr *)&peer,&peerAddrSize);
	if(ret<0){
		printf("An error occured when receiving encrypted messages\n");
		return 0;
	}
	DecryptMessages(receivedData);
	}
}//end of main
