#include"factory.h"


int wantDownLoad(int sfd){
    char buf[10];
    recv(sfd,buf,1,0);
    printf("wantDownLoad:recv:%s",buf);
    if(buf[0]=='1'){
        getFileByMmap(sfd);
    }else if(buf[0]=='0'){
        getFileBySplice(sfd);
    }
    return 0;
}


int letDownLoad(sfd,fileName){
    struct stat st;
    int ret=stat(fileName,&st);
    ERROR_CHECK(ret,-1,"stat");
    /* if(st.st_size>max){
        send(sfd,"1",1,0);
        tranFileByMmap(sfd,fileName);
    }else{
        send(sfd,"0",1,0);
        tranFileBySendfile(sfd,fileName);
    }
    return 0;*/
}






















































int main(int argc,char* argv[]){
	ARGC_CHECK(argc,3);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(sfd,-1,"socket");

	struct sockaddr_in sa;
	bzero(&sa,sizeof(sa));
	sa.sin_family=AF_INET;
	sa.sin_port=htons(atoi(argv[1]));
	sa.sin_addr.s_addr=inet_addr(argv[2]);		
	int ret=connect(sfd,(struct sockaddr*)&sa,sizeof(sa));
	ERROR_CHECK(ret,-1,"connect");
	printf("I am the client,name dilei\n");
	
    









   // char *buf=(char *)malloc(sizeof(char*));
    char command[100];
    char location[100];
    char fileName[100];

begin:
{
    bzero((void *)command,sizeof(command));
    bzero((void *)location,sizeof(location));
    bzero((void *)fileName,sizeof(fileName));
    //printf("buf=%s",buf);
    printf("\n\n enter your command:");
    scanf("%s",command);
    ret=send(sfd,command,sizeof(command),0);
	ERROR_CHECK(ret,-1,"send");
	printf("begin:I send %s\n",command);












    if(strcmp(command,"cd")==0){
        printf("cd:please type in the destination:");
        scanf("%s",location);
        ret=send(sfd,location,sizeof(location),0);
	    ERROR_CHECK(ret,-1,"send");
        
        //ret=recv(sfd,buf,100,0);
        //ERROR_CHECK(ret,-1,"recv");    
        //printf("%s",buf);    
        goto begin;
    } 














//改成虚拟文件系统之后，客户机的ls会被卡住，待修改
   else if(strcmp(command,"ls")==0){
        char buf[1024];               
        ret=recv(sfd,buf,1024,0);
        ERROR_CHECK(ret,-1,"recv");    
        puts(buf);                
        goto begin;
    } 















   else if(strcmp(command,"puts")==0){
        printf("print in the file you want to upload:");
        scanf("%s%s",location,fileName);
        send(sfd,location,sizeof(location),0);    
        //send(sfd,fileName,sizeof(fileName),0);
        
        tranFileBySendfile(sfd,fileName);
        printf("%s",fileName);
        goto begin;
    } 














    else if(strcmp(command,"gets")==0){
        printf("gets:print in location:");
        scanf("%s",location);
        ret=send(sfd,location,sizeof(location),0);
	    ERROR_CHECK(ret,-1,"send");

        printf("\n print in file name:");
        scanf("%s",fileName);
        ret=send(sfd,fileName,sizeof(fileName),0);
	    ERROR_CHECK(ret,-1,"send");

        wantDownLoad(sfd);
        goto begin;
    } 













    else if(strcmp(command,"remove")==0){
        printf("remove:print in location and filename:\n");
        //这里的换行，没有就卡不住
        scanf("%s%s",location,fileName);
        ret=send(sfd,location,sizeof(location),0);
	    ERROR_CHECK(ret,-1,"send");
        ret=send(sfd,fileName,sizeof(fileName),0);
	    ERROR_CHECK(ret,-1,"send");
       
       
       
        /*char buf[1024];
        ret=recv(sfd,buf,1024,0);
        ERROR_CHECK(ret,-1,"recv");
        printf("remove %s:mission success\n",buf);*/
        goto begin;
    } 












    else if(strcmp(command,"pwd")==0){
        char buf[1024]={0};
        //printf("buf=%s\n",buf);
        memset(buf,0,1024);
        //sleep(1);
        ret=recv(sfd,buf,sizeof(buf),0);
        ERROR_CHECK(ret,-1,"recv");
        printf("I receive %d char\n",ret);    
        printf("%s\n",buf);  
        goto begin;
    } 












    else if(strcmp(command,"mkdir")==0){        //可以
            goto begin;
    //暂时not need this functionint rmdir(const char *pathname);    
    }
    else                                         //可以
    {
        printf("        what are you talking about? enter again!\n\n");
        goto begin;
    }
}
    return 0;
} 


//BUG：1、缓冲区无法刷新，每一次都会有上一次残留数据
//     2、ls的文件之间没有空格