#include "dataprocess.hpp"
#include "debug.h"
#include <arpa/inet.h>
#include "csapp.h"
#include "TCPServer.h"


/**
*  \brief 将数组中的元素提取成word
*  \param [out] n unsigned long 存放返回结果
*  \param [in] b 输入数组指针
*  \param [in] i index位置
*  \return
*
*  \details 举例说明：b[8] = {0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF}
*  GET_ULONG_BE(n,b,4)
*  n = 0x90ABCDEF
*/
#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ] << 24 )        \
        | ( (unsigned long) (b)[(i) + 1] << 16 )        \
        | ( (unsigned long) (b)[(i) + 2] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 3]       );       \
}
#endif



#ifndef GET_SHORT_BE
#define GET_SHORT_BE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ] << 8 )        \
        | ( (unsigned long) (b)[(i) + 1]      );       \
}
#endif



/**
*  \brief GET_ULONG_BE(n,b,i)的逆过程
*
*  \param [in] n 32bit输入
*  \param [out] b
*  \param [in] b数组位置
*  \return
*
*  \details
*/
#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

#ifndef PUT_SHORT_BE
#define PUT_SHORT_BE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 8 );       \
    (b)[(i) + 1] = (unsigned char) ( (n)  );            \
}
#endif





static const int MAX_EVENT = 100;








void DataProcess::threadrun(int connectfd)
{
    struct epoll_event events[MAX_EVENT];

    struct epoll_event tep;
    int efd;
    //边沿触发方式
    tep.events = EPOLLIN | EPOLLET; 
    //tep.events = EPOLLIN; 
    tep.data.fd = connectfd;
    unsigned char nethead[16] = {0};
    unsigned char buf[1024*64] = {0};

    efd = epoll_create(MAX_EVENT);
    int res = epoll_ctl(efd, EPOLL_CTL_ADD, connectfd, &tep);
    if (res == -1)
    {   
        perror("epoll_ctl");
    }

    bool readflag = true;
    res = epoll_ctl(efd, EPOLL_CTL_ADD, connectfd, &tep);

    unsigned char resultdata[4096] = {0};
     for(int j = 0;j<32;j++){
         resultdata[j] = j;
     }



    while(1)
	{
	    //需要有一个超时退出机制 ---timeout设置
		int nfds = epoll_wait(efd, events, MAX_EVENT, -1);
        if(nfds == -1)
        {
            perror("epoll_wait");
            exit(1);
        }
        mydebug << "**********epoll_wait: " <<connectfd << " **************************" << endl;
		for (int i = 0; i<nfds; i++){
            if(events[i].data.fd == connectfd  )
            {
                int alllenth = 0;
                if(readflag){
                    int lenth = rio_readn(connectfd, (char*)nethead, 16);
                    if(lenth <= 0){
                        
                        mydebug << "delete connect:" << connectfd << endl;
                        epoll_ctl(efd,EPOLL_CTL_DEL,connectfd,&tep);
                        if (close(connectfd) == -1)
                            perror("close error");

                        
                        pthread_exit(NULL);
                        //continue;
                    }
                   GET_ULONG_BE(alllenth,nethead,8);
                   readflag = false;
                }
                alllenth = 16;  //调试的时候使用
                mydebug << "***********netdata: " <<connectfd << " **************************" << endl;
                printfRecData(nethead, 16);
                while(alllenth){
                    int index = 0;
                    int nread = 0;
                    nread = rio_readn(connectfd, (char*)&buf[index],alllenth);
                    index += nread;
                    alllenth -= nread;
                }
                readflag = true;
                mydebug << "***********bufdata: " <<connectfd << " **************************" << endl;
                printfRecData(buf, 16);
                if(1)
                {
                    mydebug << "Send start " << endl;
                    int realsend = rio_writen(connectfd, resultdata, 32);
                    mydebug << "Send finish :" << connectfd << "realsend: " << realsend << endl;
                }    
            }
		}
       
	}

}







void DataProcess::processRecData(unsigned char *data,unsigned char *out,unsigned int *lenth)
{
//    void *phSessionHandle = nullptr;
//    void *phphVsmHandle = nullptr;
//    sis_send_head_info *taskhead = (sis_send_head_info *)data;
//    unsigned short dataLength = 0;
//    unsigned int rst = 0;
//    my_printf("\n\nSSSSSSSSSSSSSSSSSSSSSSSS 开始 SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n\n");
//
//    if(taskhead->cmd== 0x0){
//        unsigned int idlenth = 32;
//        rst = SSF_ExecutetLocalCmd(taskhead,(data+16),idlenth,(sis_resp_head_info*)out);
//        debug_printf("the sis_resp_head_info data is:\n");
//        for(int i = 0;i<16;i++)
//        {
//            my_printf("0x%02x ",*(out+i));
//        }
//        my_printf("\n---------------------------\n");
//        *lenth = 16;
//    }else{
//
//        dataLength = ((sis_resp_head_info*)data)->data_length;
//        dataLength = ntohs(dataLength);
//        debug_printf("The dataLength is 0x%02x\n",dataLength);
//        my_printf("\n-------------sis_send_head_info--------------\n");
//        for(int k =0;k < 16;k++)
//        {
//            my_printf("0x%02x ",*(data+k));
//        }
//        my_printf("\n----------------------------------------------\n");
//        rst = SSF_SendCmdInit((sis_send_head_info*)data,&phSessionHandle,&phphVsmHandle);
//        if(rst != 0)
//        {
//            mydebug << "SSF_SendCmdInit error" << endl;
////            while(1);
//        }
//        rst = SSF_SendCmdUpdate(phSessionHandle,phphVsmHandle,(data+16),dataLength);
//        if(rst != 0)
//        {
//            mydebug << "SSF_SendCmdUpdate errorCode: " << rst << endl;
////            while(1);
//        }
//        rst = SSF_SendCmdFinal(phSessionHandle,phphVsmHandle);
//        if(rst != 0)
//        {
//            mydebug << "SSF_SendCmdFinal errorCode: " << rst <<  endl;
////            while(1);
//        }
//
//        rst = SSF_RespCmdInit( phSessionHandle, phphVsmHandle,(sis_resp_head_info*) out);
//        if(rst != 0)
//        {
//            mydebug << "SSF_RespCmdInit error,errorCode: " << rst << endl;
////            while(1);
//        }
//
//        unsigned int rsplenth = ntohs( ((sis_resp_head_info*)out)->data_length);
//        
//       my_printf("\n------------SSF_RespCmdInit--------------\n");
//       debug_printf("the replenth = 0x%02x\n",rsplenth);
//       for(int i=0;i<16;i++)
//       {
//           my_printf("0x%02x,",*(out+i));
//       }
//       my_printf("\n------------------------------------------\n");        
//        rst = SSF_RespCmdUpdate( phSessionHandle, phphVsmHandle,(out+16),rsplenth);
//        if(rst != 0)
//        {
//            mydebug << "SSF_RespCmdUpdate error" << endl;    //返回100001d 错误
////            while(1);
//        }
//
////        mydebug << "SSF_RespCmdUpdate rst " << rst << endl;
//
////        my_printf("\n---------SSF_RespCmdUpdate----------------\n");
////        for(int i=0;i<rsplenth;i++)
////        {
////            my_printf("0x%02x,",*(out+16+i));
////        }
////        my_printf("\n------------------------------------------\n");
//        rst = SSF_RespCmdFinal( phSessionHandle, phphVsmHandle);
//        *lenth = rsplenth + 16;           
//    }
//    my_printf("\n\nEEEEEEEEEEEEEEEEEE 结束 EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n\n");
}
void DataProcess::printfRecData(unsigned char *data,unsigned int lenth)
{
    for (int i = 0; i < lenth; i++) {
        my_printf("0x%02X,", data[i]);
    }
    my_printf("***\n");
}



