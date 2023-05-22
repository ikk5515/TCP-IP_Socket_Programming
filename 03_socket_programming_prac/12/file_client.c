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
	int sd;
	FILE *fp;
	
	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	fp=fopen("receive.dat", "wb");
	// socket function으로 만듦
	sd=socket(PF_INET, SOCK_STREAM, 0);   

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	// socket을 연결함
	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	
	// read_cnt는 읽어들인 갯수
	// 쪼개진 파일을 socket으로부터 온 것을 읽어서 계속해서 buf를 써준다.
	// 이 작업을 계속 반복한다.
	// 이 while문은 read가 0을 return할 때 끝나는데,
	// 0은 EOF를 만날 때 return한다.
	while((read_cnt=read(sd, buf, BUF_SIZE ))!=0)
		fwrite((void*)buf, 1, read_cnt, fp);
	
	puts("Received file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);	// shutdown (sd, SHUT_WR); => 원래는 이렇게 사용하는 게 맞음
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
