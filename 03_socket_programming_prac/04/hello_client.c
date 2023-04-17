#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

//.hclient 127.0.0.1 9190
// argv[0] argv[1] argv[2]
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;
	
	// argv의 개수 => argc
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	// 여기서 inet_addr은 문자열이 숫자로 바꿔주고
	// little endian을 big endian으로 변환시켜서 주소에 넣음
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
		
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");
	
	str_len=read(sock, message, sizeof(message)-1);
	if(str_len==-1)
		error_handling("read() error!");ㄴㄴㄴㄴㄴ
	
	printf("Message from server: %s \n", message);  
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
