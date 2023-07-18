#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "modbus.h"  //modbus动态库文件



#define FIFO_SERVER "/home/sundm75/pipe"
#define ERR_EXIT(m)\
	do{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

int main(int argc, char *argv[])
{
	uint16_t tab_reg[64] = {0}; //定义存放数据的数组
    modbus_t *ctx = NULL;
 
    int rc;
	int i;

    umask(0);//屏蔽掉新文件或目录不应有的访问允许权限
	int wfd=open(FIFO_SERVER,O_WRONLY); //只写方式打开管道
	if(wfd<0)
		ERR_EXIT("Open fifo error\n");

	//以串口的方式创建libmobus实例,并设置参数
	ctx = modbus_new_rtu("/dev/ttyS3", 115200, 'N', 8, 1);					
	if (ctx == NULL)
	{
    	fprintf(stderr, "Unable to allocate libmodbus contex\n");
    	return -1;
	}
	
	modbus_set_debug(ctx, 1);      //设置1可看到调试信息
	modbus_set_slave(ctx, 1);      //设置slave ID 1
	
	if (modbus_connect(ctx) == -1) //等待连接设备
	{
    	fprintf(stderr, "Connection failed:%s\n", modbus_strerror(errno));
    	return -1;
	}
	
	while (1)
	{
    	printf("\n----------------\n");
    	rc = modbus_read_registers(ctx, 0, 3, tab_reg);
    	if (rc == -1)                   //读取保持寄存器的值，可读取多个连续输入保持寄存器
    	{
			fprintf(stderr,"%s\n", modbus_strerror(errno));
    	}
    	else
    	{
            for (i=0; i<3; i++)
            {
                printf("reg[%d] = %d(0x%x)\n", i, tab_reg[i], tab_reg[i]);  
                char buf[256] = {0};
                sprintf(buf,"%d\":%5.1f}}]}",i,  (float)(tab_reg[i])/10);// 0\":31}}]}
                write(wfd,buf,strlen(buf));	
                usleep(1000000);
            }  
    	}   
	}
    modbus_close(ctx);  //关闭modbus连接
	modbus_free(ctx);   //释放modbus资源，使用完libmodbus需要释放掉
 
	return 0;
}

