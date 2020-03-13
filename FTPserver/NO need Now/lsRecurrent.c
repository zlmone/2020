#include"func.h"

int main(int argc,char *argv[]){
	/*if(strcmp(argv[1],"cd")==0){
		printf("correct");
	}
	else{
		printf("wrong");
	}*/
	DIR *dir=opendir(argv[1]);
	struct dirent *member=readdir(dir);
	printf("%s ",member->d_name);		
	/*member=readdir(dir);	
	printf("%s ",member->d_name);*/
	
    	while(member=readdir(dir)){
        	printf("%s ",member->d_name);
    	}
	return 0;
}
