#ifndef _DATAPROCESS_H_
#define _DATAPROCESS_H_

#include <iostream>
#include <mutex>





class DataProcess
{
public:
	//DataProcess();
	void  processRecData(unsigned char *data,unsigned char *out,unsigned int *lenth);
	void printfRecData(unsigned char *data,unsigned int lenth);
    void threadrun(int connectfd);





};


#endif











