#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="201944026 Kim InGi!";
	
	// 여기서는 argc의 개수가 2어야 함 (./hserver 9190)
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	// 여기서 INADDR_ANY는 자기 자신을 의미함
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	// bind는 serv_sock(socket)에 serv_addr(주소)를 엮어줌, 사용하게 해줌
	// serv_sock에 serv_addr을 bind(사용, 엮어줌)해줌
	// serv_addr의 주소를 (struct sockaddr)로 형변환해서 사용해야함
	// struct sockaddr_in serv_addr; 선언을 이렇게 했지만
	// 사용할 때는 _in을 제외하고 sockaddr로 사용해야하기 때문
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
		error_handling("bind() error"); 
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
