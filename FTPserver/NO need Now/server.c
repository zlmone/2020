#include"func.h"

void showAll(char *p){
    DIR *dir=opendir(p);
    struct dirent *member=readdir(dir);
    printf("%s  ",member->d_name);
    
    while(member=readdir(dir)){
        printf("%s ",member->d_name);
    }
}




int putFile(char *p){
    while(1);
    return 0;
}

int getFile(char *p){
    while(1);
    return 0;
}




int main(int argc,char *argv[]){
    /* if(argc!=3){
        printf("print in enough parameter");
        //return -1;
    }*/
    int ret;
    char command[10];
    char location[100];


begin:
    printf("\n enter your command:");
    scanf("%s",command);
    //printf("%s %s\n",command,location);
    if(strcmp(command,"cd")==0){
        scanf("%s",location);
        ret=chdir(location);
        printf("%s\n",getcwd(NULL,0));
        if(-1==ret){
            printf("        cd:you print in a wrong path,try again\n");
        }
        goto begin;
    } 
   else if(strcmp(command,"ls")==0){
            char buf[1024];
            strcpy(buf,getcwd(NULL,0));
            showAll(buf);
            printf("\n");
            goto begin;
    } 
   /*  else if(strcmp(command,"puts")==0){
            putFile(location);//we don't add in the funcion of choose distination for now
            goto begin;
    } 
    else if(strcmp(command,"gets")==0){
            getFile(location);//we don't add in the funcion of choose source address for now
            goto begin;
    } 
    */else if(strcmp(command,"remove")==0){
            scanf("%s",location);
            ret=remove(location);
            //remove - remove a file or directory  
            if(-1==ret){
                printf("remove:you print in a wrong path,try again\n");
            } 
            printf("mission success\n");
            goto begin;
    } 
    else if(strcmp(command,"pwd")==0){
            //char *buf[1024];
            printf("        %s\n",getcwd(NULL,0));
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
            


    return 0;
} 
