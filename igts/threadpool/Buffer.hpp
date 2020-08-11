#ifndef _BUFFER_H__
#define _BUFFER_H__

#include "MutexLock.hpp"
#include "Condition.hpp"

#include <queue>
#include <functional>
#include <list>




typedef std::function<void()> ElemType;

class Buffer{
public:
    Buffer(size_t size);
    void push(ElemType elem);
    ElemType pop();
    bool empty();
    bool full();
    void wakeup();
private:
    MutexLock   _mutex;
    Condition   _notFull;
    Condition   _notEmpty;
    size_t      _size;
    bool        _flag;
    std::queue<ElemType> _que;     //一个队列(函数指针)
    std::list<ElemType> _list; 
};


#endif
