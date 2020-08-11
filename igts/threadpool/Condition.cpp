#include "Condition.hpp"
#include "MutexLock.hpp"
#include "debug.h"




Condition::Condition(MutexLock &mutex)
    :_mutex(mutex)
{
    pthread_cond_init(&_cond,NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
    static int count = 0;
//    mydebug <<"********Buffer wait ********: " <<(count ++) << endl;
    pthread_cond_wait(&_cond,_mutex.getMutexLockPtr());
}

void Condition::notify()
{
    static int count = 0;
//    mydebug <<"********Buffer notify ********: " <<(count ++) << endl;

    pthread_cond_signal(&_cond);
}

void Condition::notifyall()
{
    pthread_cond_broadcast(&_cond);
}


