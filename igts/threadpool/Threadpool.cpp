#include "Threadpool.hpp"
#include <unistd.h>
#include <iostream>
#include "debug.h"

using namespace std;



Threadpool::Threadpool(size_t threadNum,size_t bufSize)
    :_isExit(false)
    ,_threadNum(threadNum)
    ,_bufSize(bufSize)
    ,_buff(_bufSize)
{
    _threads.reserve(_threadNum);    //reserve() 容器预留的空间
}

void Threadpool::start()
{
    //定义_threadNum个 Thread类，然后将放到sp中
    //std::bind()----->返回值是一个函数指针
    for(size_t idx = 0; idx != _threadNum; ++idx) {
        shared_ptr<Thread> sp(new Thread(std::bind(&Threadpool::processTask, this)));
        _threads.push_back(sp);
        sp->start();            //创建线程，线程运行
    } 
}

Threadpool::~Threadpool()
{
    if(!_isExit)
        stop();
}

void Threadpool::stop()
{
    if(!_isExit) {
        //等待任务队列内的任务执行结束
        while(!_buff.empty())
            sleep(1);

        _isExit = true;      //任务队列执行结束后标志线程池退出
        _buff.wakeup();
        for(auto & ele:_threads)
            ele->join();
    }
}

void Threadpool::addTask(Task task)
{
	static int i = 0;
	i++;
//	mydebug << "****add Test: " << i << endl;
    _buff.push(task);
}

Task Threadpool::getTask()
{
    return _buff.pop();
}

void Threadpool::processTask()
{
    while(!_isExit) {
        Task task = getTask();
        if(task)
        {
            task();
        }
            
    }
}


