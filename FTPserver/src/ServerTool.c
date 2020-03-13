#include"factory.h"
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


int downLoad(int sfdDep,char *fileName){
//进入文件接受模块
			int datalen;
			char buf[1024]={0};
    		recvCycle(sfdDep,&datalen,4); //接受文件长度，保存到datalen
    		recvCycle(sfdDep,buf,datalen);//接收文件名,保存到buf
    		printf("%s\n",buf);

            strcpy(fileName,buf);
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


















void sigFunc(int signum)
{
    printf("%d is coming\n",signum);
}


int tranFileBySendfile(int newFd,char* FILENAME)
{
    printf("tranFileBySendfile\n");
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


int tranFileByMmap(int newFd,char* fileNameDep)
{
    printf("tranFileByMmap\n");
    signal(SIGPIPE,sigFunc);
	//第一步，发送文件名
    train_t train;
    int ret;
    train.dataLen=strlen(fileNameDep);//发送文件名
    
    strcpy(train.buf,fileNameDep);
    send(newFd,&train,4+train.dataLen,0);
    printf("tranFileByMmap:I get file name %s\n",fileNameDep);
    int fd=open(fileNameDep,O_RDWR);
    printf("tranFileByMmap:fd=%d\n",fd);
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
    close(fd);
    return 0;
}







int letDownLoad(int sfdDep,char *fileNameDep){
    struct stat st;
    int ret=stat(fileNameDep,&st);
    if(st.st_size>max){
        send(sfdDep,"1",1,0);
        printf("letDownLoad:I get fileName %s\n",fileNameDep);
        printf("letDownLoad:sfdDep=%d\n",sfdDep);
        tranFileByMmap(sfdDep,fileNameDep);
    }else{
        send(sfdDep,"0",1,0);
        tranFileBySendfile(sfdDep,fileNameDep);
    }
    return 0;
}
































void showAll(char *p,char **allFile){
    DIR *dir=opendir(p);
    struct dirent *member=readdir(dir);
    printf("%s ",member->d_name);
    
    while(member=readdir(dir)){
        printf("%s\n",member->d_name);
        strcat(allFile,"\n");
        strcat(allFile,member->d_name);
    }
}















char *crypt(const char *key, const char *salt);

void error_quit(char *msg)
{
    perror(msg);
} 

void get_salt(char *salt,char *passwd)
{
int i,j;
//取出 salt,i 记录密码字符下标,j 记录$出现次数
for(i=0,j=0;passwd[i] && j != 3;++i)
{
if(passwd[i] == '$')
++j;
}
strncpy(salt,passwd,i-1);
}


int checkUser(char **usrNameDep)
{
    struct spwd *sp;
    char *passwd;
    char salt[512]={0};

//输入用户名密码


    passwd=getpass("请输入密码:");
//得到用户名以及密码,命令行参数的第一个参数为用户名
    if((sp=getspnam(usrNameDep)) == NULL){
        error_quit("获取用户名和密码");
        return -1;
    }
//得到 salt,用得到的密码作参数
    get_salt(salt,sp->sp_pwdp);
//进行密码验证
    if(strcmp(sp->sp_pwdp,crypt(passwd,salt)) == 0)
        printf("验证通过!\n");
    else{
        printf("验证失败!\n");
        return -1;
    }
    return 0;
}

























