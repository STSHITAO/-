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

	if(access(FIFO_SERVER, F_OK) == 0)
		remove(FIFO_SERVER);

	if(mkfifo(FIFO_SERVER,0666)<0)  //创建管道
		ERR_EXIT("Make fifo error!\n");
	int rfd=open(FIFO_SERVER,O_RDONLY); //只读方式打开管道
	if(rfd<0)
		ERR_EXIT("Open fifo error!\n");
 
    while (1)
    {
        char buf[1024] ;
        int revlen = read(rfd,buf,1024);
        if(revlen>0)
        {
            printf("收到数据为：%s \n",buf);
        }
 
		//退出循环
		if(strcmp(buf, "bye") == 0)
		{
			break;
		}
	}
	close(rfd);	

	int del_fifo = unlink(FIFO_SERVER);
	// 判断是否成功删除管道
	if (del_fifo == -1)
	{
		printf("删除 %s 管道失败: %s\n", FIFO_SERVER, strerror(del_fifo));
		return -1;
	}
	else
	{
		printf("删除 %s 管道成功: %s\n", FIFO_SERVER, strerror(del_fifo));
	}
	return 0;  
}
 

