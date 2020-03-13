#include"factory.h"


int databaseLink(MYSQL **conn,char* db)
{
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="666";
	char* database=db;
	
    

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



int DatabaseDoubleFind(MYSQL *conn,char *tableDep,char *keyItem1,char *keyContent1,char *keyItem2,char *keyContent2){
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
	int ret;

	
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
            for(t=0;t<mysql_num_fields(res);t++)
			{
				printf("%s",row[t]);
                ret=atoi(row[t]);
                printf("ret=%d\n",ret);
			}
			printf("\n");
			
		}else{
			printf("Don't find data\n");
            return -1;
		}   
		mysql_free_result(res);
        return ret;
	}
}



int DatabaseSingleFindPath(MYSQL *conn,char *tableDep,char *keyItem,char *keyContent,char *presentDirDep){
	char query[200]="SELECT path FROM ";
    strcat(query,tableDep);
    //strcat(query,"WHERE fileCode='");
    //strcat(query,mdFive);
    strcat(query," WHERE ");
    strcat(query,keyItem);
	strcat(query,"='");
	printf("DatabaseSingleFindPath:keyContent=%s\n",keyContent);
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
			//printf("num=%d\n",mysql_num_fields(res));//列数
			for(t=0;t<mysql_num_fields(res);t++)
			{
				printf("%8s ",row[t]);
			}
			printf("\n");
			strcpy(presentDirDep,row[0]);
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
}



int main(int argc,char *argv[]){
    MYSQL *dbConn;
    int retu;
    databaseLink(&dbConn,"ProjectFTP");
    retu=DatabaseDoubleFind(dbConn,"qianqi","path",argv[1],"name",argv[2]);
    printf("retu=%d\n",retu);
	char checkNode[3];
	//checkNode[0]='0'+retu;
	checkNode[0]=itoa(retu);

	printf("checkNode[0]=%c",checkNode[0]);
	char Dir[40];
	printf("\n\n\n");
	DatabaseSingleFindPath(dbConn,"qianqi","Node",checkNode,Dir);
    printf("Dir=%s\n",Dir);
	return 0;
}