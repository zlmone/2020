#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<openssl/md5.h>

int main(int argc, char* argv[]) 
{ 
     char *buf = "helloworld"; 
     char *md;  
     int i;
     md = MD5(buf,strlen(buf),NULL); 
     printf("%s\n",md);
     for (i = 0; i < strlen(md); i++)
     printf("%x", md[i]);
     printf("\n");
     return 0; 
}  

