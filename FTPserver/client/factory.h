#ifndef _FACTORY_H_
#define _FACTORY_H_
#include "head.h"

typedef struct Node{
    int newFd;//the id which system distribute to thread
    struct Node *pNext;
}Node_t,*pNode_t;

typedef struct{
    pNode_t queHead;
    pNode_t queTail;
    int queCapasity;//the max capasity the queue can have
    int queSize;    //the num of item it have,now
    pthread_mutex_t mutex;
//队列need to lock ,cause thread share the same address space
}Que_t,*pQue_t;



//que initiaize
//transfer in value of capsity,mutex(to tran out),and head,tail
void queInit(pQue_t,int);
//set to add item in it
void queInsert(pQue_t,pNode_t);
//get the item from it,maybe it can find the item you want
int queGet(pQue_t,pNode_t*);
//delete item from it,didn't have this one












typedef struct{
    Que_t que;
    pthread_cond_t cond;        //?????????????????
    pthread_t *pthid;//存储线程ID的起始地址
    int threadNum;//记录the number of threads
    short startFlag;//布尔type,used to show if ~~~ is started
}Factory_t,*pFactory_t;


typedef struct{
    int dataLen;
    char buf[1000];
}train_t;




void *threadFunc(void *p);
void Conmunicate(int sfd);




//this one is used to initialize
int factoryInit(pFactory_t,int,int);
//this one is used to start thread
int factoryStart(pFactory_t);
int tcpInit(int *sfd,char* ip,char* port);
int tranFileBySendfile(int newFd,char* FILENAME);


int getFileByMmap(int sfdDep);
int getFileBySplice(int sfdDep);


#endif
