#ifndef _EPOLL_H
#define _EPOLL_H

#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "Channel.h"
#include <list>

class Epoll
{
public:
	typedef std::list<Channel *> ChannelList;
	Epoll(int listen_fd, pthread_mutex_t* mutex, pthread_cond_t* cond);
	void set_callback(std::function<void(int)> &fcn);
	void loop(ChannelList &);
	void epoll_add_event(int epfd, int fd, int state);
	void handle_accept(int epfd, int listenfd,ChannelList & work_list);
	void set_fd_nonblocking(int fd);
	void add_to_channel(int fd, ChannelList &work_list);
	void epoll_delete_event(int fd_to_delete);

    void set_connectfd_con(int client_sockfd);

private:
	int _listenfd;
	int epfd;
	pthread_mutex_t* _mutex_ptr;
	pthread_cond_t* _cond_ptr;
	std::function<void(int)> read_callback;
};


#endif