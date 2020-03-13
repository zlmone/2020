#include "work_que.h"

void queInit(pQue_t pq,int maxN){
    bzero(pq,sizeof(Que_t));//这个为什么？？？？？
   //为什么不初始化.quesize????
    pq->queCapasity=maxN;
    pthread_mutex_init(&pq->mutex,NULL);
    //pq->mutex=1;这个not right,写上面那个
    return;
}



//use the way of linklist tailinsert 
void queInsert(pQue_t a,pNode_t b){
    if(a->queHead==NULL){
        a->queHead=b;
        a->queTail=b;
        printf("queInsert:insert successful,b->newFd=%d\n",b->newFd);
        //a->queSize=1;改为写在条件结构外，更有一般性
    }else{
        a->queTail->pNext=b;
        a->queTail=b;
        //订正：忘记修改数目了你
    }
    a->queSize++;
}


//
int queGet(pQue_t pq,pNode_t* pDel){
    if(NULL==pq->queHead)
    {
        //printf("queGet:I will return -1 then\n");
        return -1;
    }//订正：没有判断队列空
    //必须判断：不然会有dead lock问题

//you can run your on code when you available 
   /*  if(a->queSize){
        (*b)->newFd=a->queHead->newFd;
        (*b)->pNext=NULL;
        a->queHead=a->queHead->pNext;
        (a->queSize)--;
    }*/
    *pDel=pq->queHead;
    pq->queHead=pq->queHead->pNext;//get the value and change the head
    printf("queGet:insert successful,(*pDel)->newFd=%d\n",(*pDel)->newFd);
    if(NULL==pq->queHead)//now if it is null ,change the tail
    {
        pq->queTail=NULL;
    }
    pq->queSize--;
    return 0;
}