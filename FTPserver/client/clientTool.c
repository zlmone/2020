#include"factory.h"
ssize_t splice(int fd_in, loff_t *off_in, int fd_out,loff_t *off_out, size_t len, unsigned int flags);


//下载单元
int recvCycle(int sfdDep2,void *buf,int bufLen){
	char *p=(char*)buf;
	int ret;
	int total=0;
	while(total<bufLen){
		ret=recv(sfdDep2,p+total,bufLen-total,0);	
		total+=ret;		//total将会记录当前迭代的总长度
	}
	return 0;
}









int getFileByMmap(int sfdDep){
//进入文件接受模块
	printf("getFileByMmap\n");
	int datalen;
	char buf[1024]={0};
    recvCycle(sfdDep,&datalen,4); //接受文件长度，保存到datalen
    recvCycle(sfdDep,buf,datalen);//接收文件名,保存到buf
    printf("%s\n",buf);
	int fd=open(buf,O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");
    //文件应该是服务器的文件，为什么客户端可以直接打开
    


	//添加进度条环节
	off_t fileSize;
	//fileSize用来储存总大小，downLoadSize存储已经下载的大小
	recvCycle(sfdDep,&datalen,4);
	recvCycle(sfdDep,&fileSize,datalen);//这两步接收文件大小结构体
    struct timeval start,end;
    gettimeofday(&start,NULL);

	ftruncate(fd,fileSize);
	char *pMap=(char*)mmap(NULL,fileSize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
	recvCycle(sfdDep,pMap,fileSize);
	munmap(pMap,fileSize);//munmap之后才能write back

	
	
	gettimeofday(&end,NULL);
    printf("use time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);
    
    
    return 0;
}










int getFileBySplice(int sfdDep){
    printf("tranFileBySplice\n");
	int ret;
    int fd;
    int dataLen;
    char buf[1000]={0};
    recvCycle(sfdDep,&dataLen,4);
    recvCycle(sfdDep,buf,dataLen);//接收文件名
    fd=open(buf,O_CREAT|O_RDWR,0666);
    ERROR_CHECK(fd,-1,"open");
    //接收文件大小
    off_t fileSize,downLoadSize=0;
    recvCycle(sfdDep,&dataLen,4);
    recvCycle(sfdDep,&fileSize,dataLen);
    printf("fileSize=%ld\n",fileSize);
    struct timeval start,end;
    int fds[2];
    pipe(fds);
    gettimeofday(&start,NULL);
    while(downLoadSize<fileSize)
    {
        ret=splice(sfdDep,NULL,fds[1],NULL,65536,SPLICE_F_MOVE|SPLICE_F_MORE);
        ERROR_CHECK(ret,-1,"splice");
        splice(fds[0],NULL,fd,NULL,ret,SPLICE_F_MOVE|SPLICE_F_MORE);
        downLoadSize+=ret;
    }
    gettimeofday(&end,NULL);
    printf("use time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);
    return 0;
}






































//上传单元
void sigFunc(int signum)
{
    printf("%d is coming\n",signum);
}









int tranFileBySendfile(int newFd,char* FILENAME)
{
    signal(SIGPIPE,sigFunc);
    train_t train;
    int ret;
    train.dataLen=strlen(FILENAME);//发送文件名
    strcpy(train.buf,FILENAME);
    send(newFd,&train,4+train.dataLen,0);
    //发送文件大小给客户端
    
    struct stat buf;
    int fd=open(FILENAME,O_RDWR);
    fstat(fd,&buf);
    train.dataLen=sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    send(newFd,&train,4+train.dataLen,0);
    
    //发送文件内容
    ret=sendfile(newFd,fd,NULL,buf.st_size); 
    printf("sendfile ret=%d\n",ret);
    ERROR_CHECK(ret,-1,"sendfile");
    return 0;
}



/*








int tranFileByMmap(int newFd)
{
    signal(SIGPIPE,sigFunc);
	//第一步，发送文件名
    train_t train;
    int ret;
    train.dataLen=strlen(FILENAME);//发送文件名
    
    strcpy(train.buf,FILENAME);
    send(newFd,&train,4+train.dataLen,0);
    int fd=open(FILENAME,O_RDWR);
    struct stat buf;
    fstat(fd,&buf); 
    train.dataLen=sizeof(buf.st_size);
    memcpy(train.buf,&buf.st_size,train.dataLen);
    send(newFd,&train,4+train.dataLen,0);
    
    char *pMap=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
    ret=send(newFd,pMap,buf.st_size,0);
    if(-1==ret){
        return -1;
     }

    //第三步，发送结束标志
    send(newFd,&train,4,0);
    return 0;
}

*/





























