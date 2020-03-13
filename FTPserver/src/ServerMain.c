#include"factory.h"


void queInit(pQue_t pq,int maxN){
    bzero(pq,sizeof(Que_t));
    pq->queCapasity=maxN;
    pthread_mutex_init(&pq->mutex,NULL);
    return;
}




    
void queInsert(pQue_t a,pNode_t b){
    if(a->queHead==NULL){
        a->queHead=b;
        a->queTail=b;
        printf("queInsert:insert successful,b->newFd=%d\n",b->newFd);
    }else{
        a->queTail->pNext=b;
        a->queTail=b;
    }
    a->queSize++;
}





//用于获取任务
int queGet(pQue_t pq,pNode_t* pDel){
    if(NULL==pq->queHead)
    {
        //printf("queGet:I will return -1 then\n");
        return -1;
    }
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































void *threadFunc(void *p){
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
            Conmunicate(pGet->newFd);
            free(pGet);
            pGet=NULL;
            //订正：指针空间用完必须要free
        }

    }
}





































int factoryInit(pFactory_t pF,int Num,int m){
    queInit(&pF->que,m);
    pthread_cond_init(&pF->cond,NULL);

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
    }
        pFact->startFlag=1;
    return 0;
}







int tcpInit(int *sfd,char* ip,char* port)
{
    int socketFd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family=AF_INET;
    serAddr.sin_port=htons(atoi(port));
    serAddr.sin_addr.s_addr=inet_addr(ip);
    
    int reuse=1;
    int ret;
    ret=setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret=bind(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"bind");
    listen(socketFd,10);
    *sfd=socketFd;
    return 0;
}


































int main(int argc,char *argv[]){
    /* if(argc!=5){
        printf("./thread_pool_server IP PORT THREAD_NUM CAPACITY\n");
        return -1;
    }*/
    FILE *fp=fopen(argv[1],"rb");
    char ip[100],port[100],num[100],cap[100];
    fgets(ip,sizeof(ip),fp);
    fgets(port,sizeof(port),fp);
    fgets(num,sizeof(num),fp);
    fgets(cap,sizeof(cap),fp);

    printf("配置： %s %s %s %s/n",ip,port,num,cap);
//初始化阶段
    Factory_t threadInfo;
    int threadNum=atoi(strrchr(num,':')+1);
    int capacity=atoi(strrchr(cap,':')+1);
    factoryInit(&threadInfo,threadNum,capacity);
    factoryStart(&threadInfo);
   // printf("factory start over\n");
    printf("%d %d",threadNum,capacity);
    int socketFd;
    tcpInit(&socketFd,strrchr(ip,':')+1,strrchr(port,':')+1);
    printf("I am your dad\n");
    


//联网阶段   
    int newFd;
    while(1){
        newFd=accept(socketFd,NULL,NULL);
        if(threadInfo.que.queSize<threadInfo.que.queCapasity){         
            //printf("main:in main server,newFd=%d\n",newFd);
            threadInfo.que.queSize++;
            pNode_t pNew=(pNode_t)calloc(1,sizeof(Node_t));;
            pNew->newFd=newFd;
            
            

            pthread_mutex_lock(&(threadInfo.que.mutex));
            queInsert(&threadInfo.que,pNew);
            pthread_mutex_unlock(&(threadInfo.que.mutex));
            sleep(3);
            //if(threadInfo.threadNum==0)  
            //    pthread_cond_signal(&(threadInfo.cond));
            //you need to wake up the child thread every time
            //not just wake them when there is no available
            //thread can only be locked of transfer in one 宏观　time
            pthread_cond_signal(&(threadInfo.cond));
            printf("main:distribute successful\n");
        }
    }
    return 0;
}