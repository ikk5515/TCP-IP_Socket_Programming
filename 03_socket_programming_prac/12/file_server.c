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
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	// 아래 파일을 열어서 새로 receive.dat파일로 옮겨져서 만들어짐
	// 서버로부터 클라이언트가 파일을 다운로드 받음
	// rb는 그대로 읽어들이겠다는 의미 read binary
	fp=fopen("test.dat", "rb"); 
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);    
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	
	// 여기서 while문은 file하나를 정해진 사이즈(지금은 BUF_SIZE가 30)만큼 읽고
	// 잘라서 buf에 넣고 보내주는 역할을 함
	// 여기서 1은 바이트 단위
	while(1)
	{
		read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
		// 마지막에 지정된 크기보다 적게 읽었을 때 while문을 벗어남
		if(read_cnt<BUF_SIZE)
		{
			write(clnt_sd, buf, read_cnt);
			break;
		}
		// 통신용 socket에 지정된 크기만큼 buf를 전달함
		write(clnt_sd, buf, BUF_SIZE);
	}
	
	// 여기서 shutdown은 EOF를 보내기 위해 close역할(반만 닫는 역할, half close)을 하기 때문에,
	// close가 EOF가 되고, EOF를 만나 0을 return하기 때문에
	// client에서 while문을 종료하게 된다.
	/* 소켓은 입력 스트림과 출력 스트림을 만듦
	  [서버]   [클라]
	   출력 x --- 입력 x
	    |         |
	   입력 o --- 출력 o
	*/

	// SHUT_WR은 출력 스트림만 닫았기 때문에 입력으로 read는 할 수 있음
	shutdown(clnt_sd, SHUT_WR);		
	read(clnt_sd, buf, BUF_SIZE);
	printf("Message from client: %s \n", buf);
	
	fclose(fp);
	close(clnt_sd);		// shutdown(clnt_sd, SHUT_RD); => 원래 이렇게 써야 함, 이미 출력은 닫혔기 때문
	close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
