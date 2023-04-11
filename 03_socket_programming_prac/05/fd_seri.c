#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{	
	int fd1, fd2, fd3;
	// socket function을 call 하면 내부 장치를 만들어서 3번부터 차례로 return
	// socket이 아닌 open을 call 해도 resource를 만들어서 마찬가지로 차례로 return함
	// 0은 표준입력 | 1은 표준 출력 | 2는 표준 에러 출력 이므로 3부터 시작
	fd1=socket(PF_INET, SOCK_STREAM, 0);
	// 생성하려는데 실패 시 -1 값을 리턴
	// 생성 완료하면 4 라는 값을 리턴
	fd2=open("test.dat", O_CREAT|O_WRONLY|O_TRUNC); 
	fd3=socket(PF_INET, SOCK_DGRAM, 0);
	
	printf("file descriptor 1: %d\n", fd1);
	printf("file descriptor 2: %d\n", fd2);
	printf("file descriptor 3: %d\n", fd3);
	
	close(fd1);
	close(fd2);
	close(fd3);
	return 0;
}
