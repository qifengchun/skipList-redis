#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h>
#define MaxSize 1024
class Epoller
{
public:
    Epoller(int num = 5);
    void addfd(int fd);//将文件描述符加入到epoller对象中
    int setnonblocking(int fd);
    int getReadSocket();
    epoll_event* getEvents();
private:
    int _epollfd;
    epoll_event _events[MaxSize];
}
#endif // EPOLLER_H
