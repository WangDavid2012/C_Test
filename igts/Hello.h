class Hello
{
public:
	Hello(int fd);
	void handle();

private:
	int connfd;
};

