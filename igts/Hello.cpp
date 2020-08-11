#include "Hello.h"
#include "csapp.h"
#include <cstdio>
#include <pthread.h>
#include "debug.h"








Hello::Hello(int fd) : connfd(fd)
{
}

//void Hello::handle()
//{
//	char buf[MAXLINE];
//	rio_t rio;
//	Rio_readmaekinitb(&rio, connfd);
//	int nread;
//	while (nread = Rio_readlineb(&rio, buf, MAXLINE) > 0)
//	{
//		printf("I am %lu!", pthread_self());
//		printf("%s\n", buf);
//	}
//	Rio_writen(connfd, buf, nread);
//
//}





void Hello::handle()
{
//	unsigned char buf[MAXLINE];
//	rio_t rio;
//    unsigned char resultdata[4096] = {0};
//
//	int nread;
//    unsigned int alllenth = 0;
//
//    unsigned char nethead[32] = {0};
//    unsigned int lenth = 16;
//    
//    
//  
//    nread = Rio_readn(connfd,(char*)nethead,16);
//    GET_ULONG_BE(alllenth,nethead,8) 
//    mydebug << "The reclenth:" << alllenth << "threadId:" << pthread_self() << endl;;
//
//    nread = Rio_readn(connfd,(char*)buf,16);
//    my_printf("\n------------------------------------\n");
//    for(int i=0;i<16;i++)
//    {
//        my_printf("0x%02x,",nethead[i]);
//    }
//    my_printf("\n------------------------------------\n");    
//    
//    
//
//
//
//    resultdata[0] = '@';
//    resultdata[1] = 's';
//    resultdata[2] = 'i';
//    resultdata[3] = 's';
//    resultdata[4] = 'c';
//    resultdata[5] = 'o';
//    resultdata[6] = 'm';
//    resultdata[7] = '\n';
//    PUT_ULONG_BE(lenth,resultdata,8);
//    resultdata[12] = '1';
//    resultdata[13] = '1';
//    resultdata[14] = '1';
//    resultdata[15] = '1';
//    static unsigned int count =0;
//    count ++;
//    
//    PUT_ULONG_BE(count,resultdata,0);
//    PUT_ULONG_BE(count,resultdata,20);
//    PUT_ULONG_BE(count,resultdata,24);
//    PUT_ULONG_BE(count,resultdata,28);
//
//	Rio_writen(connfd, (char *)resultdata, 32);

}


















