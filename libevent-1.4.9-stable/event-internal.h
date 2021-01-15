/*************************************************************************************
 1）头文件
 主要就是event.h：事件宏定义、接口函数声明，主要结构体event的声明；
 2）内部头文件
 xxx-internal.h：内部数据结构和函数，对外不可见，以达到信息隐藏的目的；
 3）libevent框架
 event.c：event整体框架的代码实现；
 4）对系统I/O多路复用机制的封装
 epoll.c：对epoll的封装；
 select.c：对select的封装；
 devpoll.c：对dev/poll的封装;
 kqueue.c：对kqueue的封装；
 5）定时事件管理
 min-heap.h：其实就是一个以时间作为key的小根堆结构；
 6）信号管理
 signal.c：对信号事件的处理；
 7）辅助功能函数
 evutil.h 和evutil.c：一些辅助功能函数，包括创建socket pair和一些时间操作函数：加、减和比较等。
 8）日志
 log.h和log.c：log日志函数
 9）缓冲区管理
 evbuffer.c和buffer.c：libevent对缓冲区的封装；
 10）基本数据结构
 compat/sys下的两个源文件：queue.h是libevent基本数据结构的实现，包括链表，双向链表，队列等；
 _libevent_time.h：一些用于时间操作的结构体定义、函数和宏定义；
 11）实用网络库
 http和evdns：是基于libevent实现的http服务器和异步dns查询库；


 xxx-internal.h：内部数据结构和函数，对外不可见，以达到信息隐藏的目的； 
 *******************************************************************************/
#ifndef _EVENT_INTERNAL_H_
#define _EVENT_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "min_heap.h"
#include "evsignal.h"


/********************************************************************
eventop即event operator，我们可以看到它的内部封装了事件操作的名字，以及各种方法，
可以用来配合实现C语言的多态。
这里实际上将会封装I/O多路复用选项，
比如：name=epoll，然后init，add等方法就用epoll的方法，或者name=select，
用select的方法。至于怎么选，看下面。

evsel实际上会在event_base_new函数里面初始化，
会根据你在编译前的选项来选择I/O multiplexer，
我把ecent_base_new函数中选择部分截取出来如下，
以及eventops选项数组。
************************************************************************/
struct eventop {
	const char *name;
	void *(*init)(struct event_base *);
	int (*add)(void *, struct event *);
	int (*del)(void *, struct event *);
	int (*dispatch)(struct event_base *, void *, struct timeval *);
	void (*dealloc)(struct event_base *, void *);
	/* set if we need to reinitialize the event base */
	int need_reinit;
};

/***************************************************************************
Reactor——反应器
    Reactor，是事件管理的接口，内部使用event demultiplexer注册、注销事件；
    并运行事件循环，当有事件进入“就绪”状态时，调用注册事件的回调函数处理事件。
对应到libevent中，就是event_base结构体

典型的Reactor声明方式
class Reactor
{
public:
    int register_handler(Event_Handler *pHandler, int event);
    int remove_handler(Event_Handler *pHandler, int event);
    void handle_events(timeval *ptv);
    // ...
};
Event Handler——事件处理程序 


Reactor 与EventDemultiplexer  构成依赖关系。
依赖关系通常是单向的，一方是另一方的成员或者变量。


反应器中需要有两个成员，
成员一：事件处理程序---EvnetHandler 与反应器构成合成关系
成员二：事件分发功能------与Reactor是合成关系，是Reactor的一个成员

*************************************************************************/
struct event_base {
	const struct eventop *evsel;    //用来接收系统编译前设置I/O机制的名字，来自eventops
	void *evbase;                   //接收init返回的I/O多路复用的op，如epollop
	int event_count;		/* counts number of total events */         //事件总数
	int event_count_active;	/* counts number of active events */        //活跃事件数目

	int event_gotterm;		/* Set to terminate loop */
	int event_break;		/* Set to terminate loop immediately */

	/* active event management */
	struct event_list **activequeues; //activequeues[priority]是一个链表，链表中的每个结点都是优先级为priority的就绪事件event
	int nactivequeues;

	/* signal handling info */
	struct evsignal_info sig;       //专门管理信号的结构体

	struct event_list eventqueue;   //链表，保存了所有注册事件event的指针
	struct timeval event_tv;        //管理时间变量

	struct min_heap timeheap;       //用来管理定时时间的小根堆

	struct timeval tv_cache;
};

/* Internal use only: Functions that might be missing from <sys/queue.h> */
#ifndef HAVE_TAILQFOREACH
#define	TAILQ_FIRST(head)		((head)->tqh_first)
#define	TAILQ_END(head)			NULL
#define	TAILQ_NEXT(elm, field)		((elm)->field.tqe_next)
#define TAILQ_FOREACH(var, head, field)					\
	for((var) = TAILQ_FIRST(head);					\
	    (var) != TAILQ_END(head);					\
	    (var) = TAILQ_NEXT(var, field))
#define	TAILQ_INSERT_BEFORE(listelm, elm, field) do {			\
	(elm)->field.tqe_prev = (listelm)->field.tqe_prev;		\
	(elm)->field.tqe_next = (listelm);				\
	*(listelm)->field.tqe_prev = (elm);				\
	(listelm)->field.tqe_prev = &(elm)->field.tqe_next;		\
} while (0)
#endif /* TAILQ_FOREACH */

int _evsignal_set_handler(struct event_base *base, int evsignal,
			  void (*fn)(int));
int _evsignal_restore_handler(struct event_base *base, int evsignal);

#ifdef __cplusplus
}
#endif

#endif /* _EVENT_INTERNAL_H_ */
