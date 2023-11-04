#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include "epoller.h"
#include "threadpool.h"
#include "skiplist.h"
#include "command.h"

extern "C"{
#include <arpa/inet.h>
}

//定义在redis中需要使用到的数据结构
//使用STL中的map作为dict,后期可能会自己造轮子
//数据库使用一个dict表示
//定义了一个名为 myDB 的数据结构，

//它是一个模板类 SkipList 的别名。在这个别名中，
//SkipList 是一个模板类，它有两个类型参数，
//都是 std::string 类型，一个用作键（key），
//另一个用作值（value）。
typedef SkipList<std::string,std::string> myDB;

class Client;
//对命令的函数指针进行typedef
//用命令的函数指针表示一个命令集合
//typedef void (*CommandFun) (Client*,std::string,std::string&,bool&);

//服务端
class Server{
public:
    Server(int num);
    void start();
    void workerThread(int fd);//工作线程
private:
    int _listenfd;//监听进程
    struct sockaddr_in _local;//本地sock结构体

    int _port;//端口号

    //当前服务器已连接的客户端集合
    std::unordered_map<int,Client*> _clients;
    Epoller *_epoller;
    ThreadPool *_threadPool;

    void initServer();
    void processListen(int fd);

    void processSet(const Command& C, Client* c);
    void processGet(const Command& C, Client* c);
    void processDel(const Command& C, Client* c);
    void processLoad(const Command& C, Client* c);
    void processDump(const Command& C, Client* c);
    void processDisplay(const Command& C, Client* c);

};

//客户端
class Client
{
public:
    int sockfd;
    struct sockaddr_in client_addr;
    //客户端对应的数据库
    myDB db;
}

#endif // SERVER_H
