#include "IGTP_Base.h"

#define MAX_LEN 2048

class IGTP : IGTP_Base
{
private:
	int _connfd;
	int version;
	int action;
	char request[MAX_LEN];
	char response[MAX_LEN];
public:
	IGTP(int fd) :_connfd(fd) {}

	void handle();

	void accept_request();
	void handle_request();
	void send_response();
};

