#ifndef _FACTORY_H_
#define _FACTORY_H_
#include "head.h"
#include "md5.h"
#include "work_que.h"
#define max 1048576




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

int checkUser(char **usrNameDep);





//serverTool

int recvCycle(int sfdDep2,void *buf,int bufLen);
int downLoad(int sfdDep,char *fileName);


int letDownLoad(int sfdDep,char *fileNameDep);

void showAll(char *p,char **allFile);

int checkUser(char **usrNameDep);


//Database
int compute_file_md5(int fd , char *md5_str);
//int databaseLink(MYSQL **conn);
int DatabaseChangeDir(MYSQL *conn,char *table,char *path);
int DatabaseShowall(MYSQL *conn,char *table);
int DatabasePuts(MYSQL *conn,char *table,char *fileName,char *mdFive);
int DatabaseGets(MYSQL *conn,char *table,char *mdFive);
int DatabaseRemove(MYSQL *conn,char *table,char* md5sum);
//DatabaseWrite(path,fileName,buf,size);
int DatabaseSingleFind(MYSQL *conn,char *tableDep,char *keyItem,char *keyContent,char *wantToFind,char *presentDirDep);
#endif
