#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t clnt_adr_sz;
	
	struct sockaddr_in serv_adr, clnt_adr;
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	// udp에서 socket은 SOCK_DGRAM
	serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock==-1)
		error_handling("UDP socket creation error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	//여기에는 9190이 들어감
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	while(1) 
	{
		clnt_adr_sz=sizeof(clnt_adr);

		// recvfrom => udp에서는 주소를 보낼 때마다 주소를 지정해줌
		// 클라이언트의 주소가 clnt_adr, 메시지가 message에, 어느 포트에서 왔는지 sockaddr에 들어감
		// 받을 때는 &clnt_adr_sz에 & 가 필요하지만 보낼때는 필요없음
		str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, 
								(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
		// printf("IP: %d PORT: %d\n", ntohl(clnt_adr.sin_addr.s_addr), ntohs(clnt_adr.sin_port));
		// 보낼 때는 &가 필요없다.
		sendto(serv_sock, message, str_len, 0, 
								(struct sockaddr*)&clnt_adr, clnt_adr_sz);
		message[str_len] = 0;
		printf("%s \n", message);
		// printf("IP: %d PORT: %d\n", ntohl(clnt_adr.sin_addr.s_addr), ntohs(clnt_adr.sin_port));
	}	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
