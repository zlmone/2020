#ifndef _WORK_QUE_
#define _WORK_QUE_

#include"head.h"

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

#endif