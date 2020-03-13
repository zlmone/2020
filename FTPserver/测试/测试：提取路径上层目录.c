#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//提取最高目录
int rootExtract(char *str0,char *output){
    char *str1=(char*)malloc(sizeof(char*));
    strcpy(str1,str0);
    printf("str1=%s\n",str1);     
    char *str2=strchr(str1,'/');
    printf("str2=%s\n",str2);
   // char *str3=(char*)malloc(sizeof(char*));
    //long i=strlen(str1)-strlen(str1);
        
    long len1=strlen(str1);
    long len2=strlen(str2);
    long i=len1-len2;
    printf("len1=%ld,len2=%ld,i=%ld\n",strlen(str1),strlen(str2),i);
    strncpy(output,str1,i);
    printf("str3=%s",output);
    return 0;

}

//提取就近目录
int ReverseExtract(char *str0,char *output,char *formerPath){
    char *str2=strrchr(str0,'/');
    strcpy(output,str2+1);
    
    
    long len1=strlen(str0);
    long len2=strlen(str2);
    long i=len1-len2;
    printf("len1=%ld,len2=%ld,i=%ld\n",strlen(str0),strlen(str2),i);
    strncpy(formerPath,str0,i);
    printf("formerPath=%s",formerPath);
    return 0;
}



int main(int argc,char *argv[]){
    char *Dir=(char*)malloc(sizeof(char*));

    char *Path=(char*)malloc(sizeof(char*));
    ReverseExtract(argv[1],Dir,Path);
    printf("Dir=%s\n path=%s\n",Dir,Path);
}