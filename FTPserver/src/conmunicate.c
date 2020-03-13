#include"factory.h"
//工作程序函数
void Conmunicate(int sfd){
    int ret;
    char userName[100];
    //char passWd[100];
log://登陆模块
    printf("please log in\n");
    printf("    user name:");
    scanf("%s",userName);
    /*printf("\n  password:");
    scanf("%s",passWd);*/
    ret=checkUser(&userName);
    if(ret==-1){
        printf("can't find the user,try again!\n");
        goto log;
    }
    






    time_t t=time(NULL);/*
    struct tm *t=localtime(NULL); */
    char *s=ctime(&t);
    printf("%s\n",s);
    //添加：连接数据库
    MYSQL *dbConn;
    databaseLink(&dbConn);
    //修改：通过数据库存储，日志文件
    //DatabaseWrite(path,fileName,buf,size);
    int fd=open(userName,O_CREAT|O_APPEND|O_RDWR);
    write(fd,"log in time:",12);
    write(fd,s,30);
    //write(fd,"\n",4);
    char command[100];
    char location[100];

    //char rootDir="mainDir";
    //char Dir[20]={0};
    char fileName[100];
    char *currPathNode[3];
    strcpy(currPathNode,"0");









begin:
    printf("\n");
    printf("enter your command:");
    //scanf("%s",command);
    ret=recv(sfd,command,sizeof(command),0);
    ERROR_CHECK(ret,-1,"recv");
    printf("I recv %s\n",command); 
    //修改：写入数据库日志   
    write(fd,"\n",2);
    write(fd,command,strlen(command));
    printf("sizeof(command)=%ld\n",sizeof(command));
    write(fd," ",2);
    strcpy(location,"mainDir");
    printf("begin:command=%s\n  ",command);







    if(strcmp(command,"cd")==0){
        printf("cd:please type in the destination:\n");
        ret=recv(sfd,location,100,0);
        ERROR_CHECK(ret,-1,"recv");//收信号       
        printf("conmunicate:I recv loca of %s\n",location);
        //修改：数据库内切换目录
        DatabaseChangeDir(dbConn,userName,location,currPathNode);

        /*char *Dir=(char*)malloc(sizeof(char*));
	    char prePath[40];
	//char *currPath=(char*)malloc(sizeof(char*));
	    ReverseExtract(location,Dir,prePath);
	    ret=DatabaseDoubleFindNode(dbConn,userName,"path",prePath,"name",Dir,currPathNode);
        */
        //DatabaseWrite(path,fileName,buf,size);
        printf("currPathNode=%s\n",currPathNode);
        
        if(-1==currPathNode){
            printf("        cd:you print in a wrong path,try again\n");
        }
        //ret=send(sfd,getcwd(NULL,0),strlen(getcwd(NULL,0)),0);
	    //ERROR_CHECK(ret,-1,"send");

        ret=write(fd,location,sizeof(location));
        write(fd,"\n",2);
        goto begin;
    } 






   else if(strcmp(command,"ls")==0){
        char buf[1024];
        memset(buf,0,sizeof(buf));
        char path[100];
        strcpy(path,getcwd(NULL,0));
        showAll(path,&buf);//buf要保存DatabaseShowall打印出来的东西
            //getcwd(NULL,0);
        //DatabaseShow(dbNAME);
        ret=send(sfd,buf,strlen(buf),0);//send结果
	    ERROR_CHECK(ret,-1,"send");
        printf("\n");
        DatabaseShowall(dbConn,location);
        write(fd,"\n",2);

        ret=write(fd,"     command result:",21);
        printf("buf=%s\n",buf);
        ret=write(fd,buf,strlen(buf));
        write(fd,"\n",2);
        //DatabaseWrite(path,fileName,buf,size);
        goto begin;
    } 







    else if(strcmp(command,"puts")==0){
        printf("I will receive your upload");
        ret=recv(sfd,location,sizeof(location),0);
        ERROR_CHECK(ret,-1,"recv");
        
        char Name[100]={0};
        downLoad(sfd,Name);
        //获取md5码
        int fdTemp=open(Name,O_CREAT|O_RDWR);
        char *md5sum=(char*)malloc(sizeof(char*));
        compute_file_md5(fdTemp,md5sum);
        DatabasePuts(dbConn,location,Name,md5sum);


        ret=write(fd,Name,sizeof(Name));
        write(fd,"\n",2);        
        ret=write(fd,"  mission success",18);
        write(fd,"\n",2);
        write(fd,"\n",2);
        goto begin;
    } 







    else if(strcmp(command,"gets")==0){
        printf("gets:print in location\n");
        ret=recv(sfd,location,100,0);
        ERROR_CHECK(ret,-1,"recv");
        printf("print in file name\n");
        ret=recv(sfd,fileName,100,0);
        ERROR_CHECK(ret,-1,"recv");
        printf("do you want %s?\n",fileName);

        //DatabaseGets(path,fileName,md5sum);
        if(strcmp(location,"0")!=0){
            ret=chdir(location);
            if(-1==ret){
                printf("        cd:you print in a wrong path,try again\n");
            }
        }
        letDownLoad(sfd,fileName);//we don't add in the funcion of choose source address for now           
        //printf("conmunicate:")

        ret=write(fd,location,sizeof(location));
        ret=write(fd,fileName,sizeof(fileName));
        write(fd,"\n",2);        
        ret=write(fd,"  mission success",18);
        write(fd,"\n",2);
        write(fd,"\n",2);
        //DatabaseWrite(path,fileName,buf,size);
        goto begin; 
    } 






    else if(strcmp(command,"remove")==0){
remove:
        printf("remove:please type in the destination:\n");
        ret=recv(sfd,location,100,0);
        ERROR_CHECK(ret,-1,"recv");
        ret=recv(sfd,fileName,100,0);
        ERROR_CHECK(ret,-1,"recv");

        int fdTemp=open(fileName,O_CREAT|O_RDWR);
        char *md5sum=(char*)malloc(sizeof(char*));
        compute_file_md5(fdTemp,md5sum);
        ret=DatabaseRemove(dbConn,location+1,md5sum);
        //可以加上判断
        if(-1==ret){
            printf("remove:you print in a wrong path or filename,try again\n");
            goto remove;
        } 
        printf("mission success\n");

        //之后要改成写日志
        ret=write(fd,location,sizeof(location));
        write(fd," ",2);
        ret=write(fd,fileName,sizeof(fileName));
        write(fd,"\n",2);
        write(fd,"\n",2);
        goto begin;
    } 









    else if(strcmp(command,"pwd")==0){
        char Dir[10]={0};
        char path[100]={0};


        DatabaseSingleFind(dbConn,userName,"Node",currPathNode,"path",path);
        printf("Dir=%s\n",Dir);
        DatabaseSingleFind(dbConn,userName,"Node",currPathNode,"Name",Dir);
        strcat(path,"/");
        strcat(path,Dir);
        
        ret=send(sfd,path,strlen(path),0);
        ERROR_CHECK(ret,-1,"send");
        //sleep(1);
        printf("I send %s\n",path);

        //DatabasePwd();
        write(fd,"\n",2);
        ret=write(fd,"     command result:",21);
        ret=write(fd,path,strlen(path));
        write(fd,"\n",2);
        goto begin;
    } 









    else if(strcmp(command,"mkdir")==0){        //可以
        goto begin;
    //暂时not need this functionint rmdir(const char *pathname);    
    }
    else if(strcmp(command,"quit")==0){
        write(fd,"\n",2);
        goto end;

    }








    else                                         //可以
    {
        char wrong[51]="        what are you talking about? enter again!\n\n";
        printf("%s",wrong);
        write(fd,wrong,strlen(wrong));
        write(fd,"\n",2);
        goto begin;
    }










end:
    printf("I will quit");
    t=time(NULL);/*
    struct tm *t=localtime(NULL); */
    s=ctime(&t);
    printf("%s\n",s);
    write(fd,"log out time:",13);
    write(fd,s,30);
    write(fd,"\n\n\n",6);
    close(fd);
    mysql_close(dbConn);
    return;
} 
