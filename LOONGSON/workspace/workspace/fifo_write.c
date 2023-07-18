#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define FIFO_SERVER "/home/sundm75/pipe"
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)
 
int main(int argc, char const *argv[])
{
    umask(0);//屏蔽掉新文件或目录不应有的访问允许权限

	int wfd=open(FIFO_SERVER,O_WRONLY); //只写方式打开管道
	if(wfd<0)
		ERR_EXIT("Open fifo error\n");

    while (1)
    {
        char buf[1024] = {0};
		printf("请输入需要发送的数据：");
        scanf("%s",buf);
        write(wfd,buf,strlen(buf));
    	
		//退出循环
		if(strcmp(buf, "bye") == 0)
		{
			break;
		}
	}
	close(wfd);	
	return 0;   
}
