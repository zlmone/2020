#include"factory.h"

void *threadFunc(void *p){
    
    //pFactory_t pFact=(pFactory_t)malloc(sieof(pFactory_t));
    //no need to apply for space again
    
    pFactory_t pThreadInfo=(pFactory_t)p;
    pQue_t pq=&pThreadInfo->que;
    if(pq->queHead==NULL){
        printf("threadFunc:pq->queHead=NULL\n");
    }
    pNode_t pGet;
    int getTaskSuccess;
    //printf("threadFunc:child thread was born\n");

    while(1){
        pthread_mutex_lock(&pq->mutex);
        if(!pq->queSize)
        {   
            //没有任务时会在wait中卡住,等待被唤醒
            pthread_cond_wait(&pThreadInfo->cond,&pq->mutex);
            printf("threadFunc:I wait a task\n\n\n");
        }
        getTaskSuccess=queGet(pq,&pGet);//拿任务
        pthread_mutex_unlock(&pq->mutex);
        if(!getTaskSuccess)
        {
            printf("threadFunc:I got a task\n\n\n");
            tranFile(pGet->newFd);
            free(pGet);
            pGet=NULL;
            //订正：指针空间用完必须要free
        }
        //如果有任务
   /*      if((pThreadInfo->que).queSize){
            pNode_t newTask;
            queGet(pThreadInfo->que,&newTask);
            pthread_mutex_unlock(&(pThreadInfo->que).mutex));
            
            
            tranFile(newTask->newFd);  
        }else{
            //you are totally wrong,the thread must be distributed
            //even if there is no task ,the thread must wait until there is task
            pthread_cond_wait(&(pThreadInfo->que.mutex));
            pthread_mutex_unlock(&(pThreadInfo->que).mutex));
        }
        */
    }
}



int factoryInit(pFactory_t pF,int Num,int m){
    queInit(&pF->que,m);//m传入队列容量
    
    pthread_cond_init(&pF->cond,NULL);
    //订正：有两种建立方法，这种叫动态法

    pF->pthid=(pthread_t*)calloc(Num,sizeof(pthread_t));
    pF->threadNum=Num;
    pF->startFlag=0;
    return 0;
}


//为什么需要启动标志，还会有好几个factory？
int factoryStart(pFactory_t pFact){
//订正：要判断是否启动
    if(!pFact->startFlag)
    {
        for(int i=0;i<pFact->threadNum;++i){
            /* if(pFact->que.queHead==NULL){
                printf("factoryStart:pFact->que.queHead=NULL\n");
            }*/
            pthread_create(pFact->pthid+i,NULL,threadFunc,pFact);
        //订正：每次的thread指针要不能the same，同时thread函数需要有传入参数
        }
        pFact->startFlag=1;//想到了，完美
    }
    return 0;
}