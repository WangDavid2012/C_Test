#include "Epoll.h"
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <sched.h>



#include "debug.h"
#include "dataprocess.hpp"
static const int MAX_EVENT = 100;

Epoll::Epoll(int listen, pthread_mutex_t * mutex, pthread_cond_t * cond) :
	_listenfd(listen), _mutex_ptr(mutex), _cond_ptr(cond)
{

}

void Epoll::loop(ChannelList & work_list)
{
	epfd  = epoll_create(1024);

	struct epoll_event events[MAX_EVENT];
	epoll_add_event(epfd, _listenfd, EPOLLIN);	

    
	while (1)
	{
		int nfds = epoll_wait(epfd, events, MAX_EVENT, -1);
		for (int i = 0; i<nfds; i++)
		{
		    if( !(events[i].data.fd & EPOLLIN))
            {
                 mydebug << "continue" << endl;
                 continue;
            }      
               
			if (events[i].data.fd == _listenfd)
			{
			    //需要与线程关联起来
				handle_accept(epfd, events[i].data.fd,work_list);
			}
		}
	}

}

void Epoll::add_to_channel(int fd, ChannelList & work_list)
{
//	Channel * new_one = new Channel(fd);
//	new_one->set_read_flag(true);
//    
//	std::function<void()> fct = [=]() {read_callback(fd); };
//	new_one->set_read_callback(fct);
//	std::function<void(int)> fct_int = std::bind(&Epoll::epoll_delete_event, this, std::placeholders::_1);
//	new_one->set_complete_callback(fct_int);
//
//	pthread_mutex_lock(_mutex_ptr);
//	work_list.push_back(new_one);
//	pthread_cond_signal(_cond_ptr);
//	pthread_mutex_unlock(_mutex_ptr);
}


//void Epoll::add_to_channel(int fd, std::thread &thread)
//{
//	Channel * new_one = new Channel(fd);
//	new_one->set_read_flag(true);
//    
//	std::function<void()> fct = [=]() {read_callback(fd); };
//	new_one->set_read_callback(fct);
//	std::function<void(int)> fct_int = std::bind(&Epoll::epoll_delete_event, this, std::placeholders::_1);
//	new_one->set_complete_callback(fct_int);
//
//	pthread_mutex_lock(_mutex_ptr);
//	work_list.push_back(new_one);
//	pthread_cond_signal(_cond_ptr);
//	pthread_mutex_unlock(_mutex_ptr);
//}








/*
int Epoll::http_epoll_create(int size)
{
	int epoll_fd;
	epoll_fd = epoll_create(size);

	if (-1 == epoll_fd)
	{
		perror("epoll_create");
		exit(-1);
	}

	return epoll_fd;
}
*/

void Epoll::epoll_add_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		perror("EPOLL_CTL_ADD");
		exit(-1);
	}
}

void Epoll::epoll_delete_event(int fd_to_delete)
{
//	struct epoll_event ev;
//	ev.events = EPOLL_CTL_DEL;
//	ev.data.fd = fd_to_delete;
//    mydebug << "delete connect:" << fd_to_delete << endl;
//	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd_to_delete, &ev) == -1)
//	{
//		perror("EPOLL_CTL_DEL");
//		exit(-1);
//	}
//	close(fd_to_delete);
}

void Epoll::handle_accept(int epfd, int listenfd,ChannelList & work_list)
{
	int conn_sock;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(sockaddr_in);
    mydebug << "epfe:" << epfd << " listenfd:"<< listenfd << endl;
    
	if ((conn_sock = accept(listenfd, (sockaddr *)&clientaddr, &addrlen)) > 0)
//        while ((conn_sock = accept(listenfd, (sockaddr *)&clientaddr, &addrlen)) > 0)
	{
		debug_printf("Client from %s connected!\n", inet_ntoa(((struct sockaddr_in *)&clientaddr)->sin_addr));
		set_fd_nonblocking(conn_sock);
        set_connectfd_con(conn_sock);
		
	}
    mydebug << "the Connect fd: " << conn_sock << endl;
	if (conn_sock == -1)
	{
		if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
		{
		    mydebug << "accept error" << endl;
			perror("accept");
		}
	}
    
	Channel * new_one = new Channel(conn_sock);
	new_one->set_read_flag(true);

    
	std::function<void()> fct = [=]() {read_callback(conn_sock); };
	new_one->set_read_callback(fct);
	std::function<void(int)> fct_int = std::bind(&Epoll::epoll_delete_event, this, std::placeholders::_1);
	new_one->set_complete_callback(fct_int);

//    std::function<void(int)> fread_int = std::bind(&DataProcess::threadrun, dataProcess,std::placeholders::_1);
//    new_one->set_read_callback(fread_int);


	pthread_mutex_lock(_mutex_ptr);
	work_list.push_back(new_one);
	pthread_cond_signal(_cond_ptr);
	pthread_mutex_unlock(_mutex_ptr);

	return;
}


void Epoll::set_callback(std::function<void(int)> &fcn)
{
	read_callback = fcn;
}

void Epoll::set_fd_nonblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
	{
		perror("fcntl: F_GETFL");
		exit(-1);
	}
	flags |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flags);
	if (ret < 0)
	{
		perror("fcntl");
		exit(-1);
	}
}

//设置连接的Fd的属性
void Epoll::set_connectfd_con(int client_sockfd)
{
    struct linger timeout;
    timeout.l_onoff = 1;
    timeout.l_linger = 0;
    
    
    setsockopt(client_sockfd, SOL_SOCKET, SO_LINGER, (void *) &timeout, sizeof(timeout));
    int keepAlive = 1 ;  //开启keepAlive属性
    setsockopt(client_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));

    int keepIdle = 300; //如果在300秒内没有数据来往，则进行探测
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));

    int keepCount = 3;   //探测的次数
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPCNT, (char *) &keepCount, sizeof(keepCount));

    int keepIntvl = 10;  //探测时间间隔是10s
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPINTVL, (void*)&keepIntvl, sizeof(keepIntvl));

    //设置后网络端口，使用该socket读写时立即失败，并返回ETIMEOUT错误


}






