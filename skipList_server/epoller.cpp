#include "epoller.h"
#include <fcntl.h>
#include <iostream>
Epoller::Epoller(int num):_epollfd(epoll_create(num)),_events{}
{

}

void Epoller::addfd(int fd){
    epoll_event event;

    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;//设置为边缘模式

    if(fd<0){
        std::cout<<"event create error"<<std::endl;
    }

    epoll_ctl(_epollfd,EPOLL_CTL_ADD,fd,&event);//将fd添加到epoll树中管理
}

int Epoller::setnonblocking(int fd){//设置非阻塞
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fctrl(fd,new_option);
    return old_option;
}

int Epoller::getReadSocket(){
    int ret = epoll_wait(_epollfd,_events,5,-1);
    if(ret<0) std::cout<<"don't have read socket\n";
    return ret;
}

epoll_event *Epoller::getEvents(){
    return _events;
}
