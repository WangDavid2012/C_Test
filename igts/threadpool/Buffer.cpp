#include "Buffer.hpp"
#include "MutexLock.hpp"
#include "debug.h"




Buffer::Buffer(size_t size)
    :_mutex()                  //新建一个mutex类
    ,_notFull(_mutex)          //创建一个_notFull类          
    ,_notEmpty(_mutex)         //创建一个_notEmpty类
    ,_size(size)               //_size赋值
    ,_flag(true)               //_flag赋值
{ 
}

bool Buffer::full()
{
    return _que.size()==_size;
}

bool Buffer::empty()
{
    return _que.size() == 0;
}

//运行于生产者线程
void Buffer::push(ElemType elem)
{
    {
        MutexLockGuard guard(_mutex);   
        static int count = 0;
        while(full())                   
        {
//            mydebug << "****************buffer full ***************" << endl;
            _notFull.wait();
        }
//         mydebug << "******buffer push,que_size: " << _que.size() << "**count**:"<< (++count) << endl;   
        _que.push(elem);
    }   
    _notEmpty.notify();                
}

//运行于消费者线程
ElemType Buffer::pop()
{
//    mydebug << "******buffer wait mutex start"  << endl;  
    MutexLockGuard guard(_mutex);   //不能创建临时变量,避免异常退出导致死锁
    static int count = 0;
//    mydebug << "******buffer wait mutex end"  << endl;  
    while(_flag && empty())
    {
//        mydebug << "******buffer pop,que_size: " << _que.size() << "****" << endl;  
        _notEmpty.wait();
    }
        
    if(_flag) {
        ElemType ret = _que.front();
        _que.pop();
        _notFull.notify();
//         mydebug << "******buffer pop,que_size: " << _que.size() << "**flag**:" << _flag <<  "**count**:"<< (++count) << endl; 
        return ret;
    } 
    else
    {
//         mydebug << "******buffer pop nullptr: " << _que.size() << "**flag**:" << _flag  << endl;   
         return nullptr;
    }
       
}

void Buffer::wakeup()
{
    _flag = false;
    _notEmpty.notifyall();
}















