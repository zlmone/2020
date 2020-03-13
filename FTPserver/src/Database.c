#include"factory.h"




int databaseLink(MYSQL **conn)
{
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="666";
	char* database="ProjectFTP";
	
    

	*conn=mysql_init(NULL);
	if(!mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(*conn));
        return 0;
	}else{
		printf("Connected...\n");
        return -1;
	}
}















int DatabasePuts(MYSQL *conn,char *table,char *fileName,char *mdFive)
{
    char query[200]="INSERT INTO ";
    strcat(query,table+1);
    printf("DatabasePut:table=%s\n",table+1);
    printf("md5num=%s,fileName=%s\n",mdFive,fileName);
    strcat(query,"(fileName,fileCode) VALUES('");
    strcat(query,fileName);
    strcat(query,"','");
    strcat(query,mdFive);
    strcat(query,"');");
	printf("query=%s\n",query);
    
    int t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }
    else{
		printf("insert success\n");
        return 0;
	}
}
















int DatabaseRemove(MYSQL *conn,char *table,char* mdFive){
    char query[200]="DELETE FROM ";
    strcat(query,table);
    strcat(query," WHERE fileCode='");
    strcat(query,mdFive);
    strcat(query,"';");
	printf("query=%s\n",query);

    int t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
	}else{
		printf("DELETE success\n");
        return 0;
	}
}
//int DatabasePwd(){




















int DatabaseGets(MYSQL *conn,char *table,char *mdFive){
    char query[200]="SELECT * FROM ";
    strcat(query,table);
    //strcat(query,"WHERE fileCode='");
    //strcat(query,mdFive);
    strcat(query," WHERE fileCode='");
	strcat(query,mdFive);	


    int t=mysql_query(conn,query);
	printf("query=%s t=%d\n",query,t);
    MYSQL_RES *res;
	MYSQL_ROW row;
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			row=mysql_fetch_row(res);	
				//printf("num=%d\n",mysql_num_fields(res));//列数
			for(t=0;t<mysql_num_fields(res);t++)
			{
				printf("%8s ",row[t]);
			}
			printf("\n");
			
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	return 0;

}









/*注释掉了的，暂时没有鸟用

//提取给定路径的最高目录，nextPath,output为传出参数，已经验证
int extract(char *path,char *nextPath,char *output){
    char *str1=(char*)malloc(sizeof(char*));
    strcpy(str1,path);
    printf("str1=%s\n",str1);     
    char *str2=strchr(str1,'/');
    printf("str2=%s\n",str2);
   // char *str3=(char*)malloc(sizeof(char*));
    //long i=strlen(str1)-strlen(str1);
    strcpy(nextPath,str2+1);


    long len1=strlen(str1);
    long len2=strlen(str2);
    long i=len1-len2;
    printf("len1=%ld,len2=%ld,i=%ld\n",strlen(str1),strlen(str2),i);
    strncpy(output,str1,i);
    printf("str3=%s",output);
    return 0;

}*/




//pwd功能
int DatabaseSingleFind(MYSQL *conn,char *tableDep,char *keyItem,char *keyContent,
												char *wantToFind,char *presentDirDep){
	printf("I am in DatabaseSingleFind now\n");
	char query[200]="SELECT ";
    strcat(query,wantToFind);
	//keyi
	strcat(query," FROM ");
	strcat(query,tableDep);
    strcat(query," WHERE ");

	printf("DatabaseSingleFindPath:keyContent=%s\n",keyContent);
	printf("DatabaseSingleFindPath:keyItem=%s\n",keyItem);
    strcat(query,keyItem);
	strcat(query,"='");
	strcat(query,keyContent);
	strcat(query,"';");
    int t=mysql_query(conn,query);
	printf("DatabaseSingleFindPath:query=%s \n t=%d\n",query,t); 
	
	
	MYSQL_RES *res;
	MYSQL_ROW row;
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			row=mysql_fetch_row(res);	
			printf("num=%d\n",mysql_num_fields(res));//列数
			/*for(t=0;t<mysql_num_fields(res);t++)
			{
				printf("%8s ",row[t]);
			}
			printf("\n");*/
			strcpy(presentDirDep,row[0]);
			printf("presentDirDep=%s\n",presentDirDep);
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
		return 0;
	}
}

































//3.3	cd功能

int DatabaseDoubleFindNode(MYSQL *conn,char *tableDep,
				char *keyItem1,char *keyContent1,char *keyItem2,char *keyContent2,char *outComeDep){
	char query[200]="SELECT Node FROM ";
    strcat(query,tableDep);
    //strcat(query,"WHERE fileCode='");
    //strcat(query,mdFive);
    strcat(query," WHERE ");
    strcat(query,keyItem1);
	strcat(query,"='");
	strcat(query,keyContent1);
	strcat(query,"' AND ");
	strcat(query,keyItem2);
	strcat(query,"='");
	strcat(query,keyContent2);
	strcat(query,"';");
    int t=mysql_query(conn,query);
	printf("query=%s t=%d\n",query,t); 
	//int ret;

	
	MYSQL_RES *res;
	MYSQL_ROW row;
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
            row=mysql_fetch_row(res);    
			//while((row=mysql_fetch_row(res))!=NULL)
			
			printf("num=%d\n",mysql_num_fields(res));//列数				
            //for(t=0;t<mysql_num_fields(res);t++)
			
			printf("%s\n",row[0]);
			printf("outComeDep=%s\n",outComeDep);
			strcpy(outComeDep,row[0]);
			printf("outComeDep=%s\n",outComeDep);
			
		}else{
			printf("Don't find data\n");
            return -1;
		}   
		mysql_free_result(res);
        return 0;
	}
}



//参数：数据库指针，当前表，关键项目1，查询条件1，关键项目2，查询条件2
//返回:满足条件项目的node(保证唯一）
//还没添加第二关键字




int ReverseExtract(char *str0,char *output,char *formerPath){
    printf("ReverseExtract:input in %s\n",str0);
	char *str2=strrchr(str0,'/');
    strcpy(output,str2+1);
    printf("ReverseExtract:cut to %s\n",str2);
    
    size_t len1=strlen(str0);
    size_t len2=strlen(str2);
    size_t i=len1-len2;
    printf("ReverseExtract:len1=%ld,len2=%ld,i=%ld\n",strlen(str0),strlen(str2),i);
    strncpy(formerPath,str0,i);
    printf("ReverseExtract:formerPath=%s,its len=%ld\n",formerPath,strlen(formerPath));
	return 0;
}




//presentDir为传出参数，传出当前目录
//返回值为目录对应的Node,可以用于表示当前目录
int DatabaseChangeDir(MYSQL *conn,char *table,char *path,char* outCome){
	//如果是开头是/..，添加返回上层目录功能
	char *Dir=(char*)malloc(sizeof(char*));
	char prePath[40];
	//char *currPath=(char*)malloc(sizeof(char*));
	ReverseExtract(path,Dir,prePath);
	int ret=DatabaseDoubleFindNode(conn,table,"path",prePath,"name",Dir,outCome);
	printf("DatabaseChangeDir:outCome=%s\n",outCome);
	return ret;	
}






















//这个功能较差，舍弃
int DatabaseShowall(MYSQL *conn,char *table){
    char query[200]="SELECT * FROM ";
    strcat(query,table);
    //strcat(query,"WHERE fileCode='");
    //strcat(query,mdFive);
    strcat(query,";");	
    int t=mysql_query(conn,query);
	printf("query=%s t=%d\n",query,t);
    MYSQL_RES *res;
	MYSQL_ROW row;
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
        return -1;
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)
				{
					printf("%8s ",row[t]);
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	return 0;

}


