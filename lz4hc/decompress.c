#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lz4hc.h"
unsigned char out[51*1024];
short out_len=0;
int main(int argc,char* argv[]){
    if(argc<2){
        printf("please enter log_file name");
    }
    int file_len = 11+ strlen(argv[1]);
    char filename[file_len];
    sprintf(filename,"decompress_%s",argv[1]);
    FILE* fd = fopen(argv[1],"rb+");
    FILE* out_fd = fopen(filename,"a+");
    if(fd!=NULL){
    printf("open!\n");
    fseek(fd,-sizeof(short),SEEK_END);
    short count;
    fread(&count,sizeof(short),1,fd);
    short len[count];
    fseek(fd,-sizeof(short)-count*sizeof(short),SEEK_CUR);
    for(int i=0;i<count;i++){
        fread(&len[i],sizeof(short),1,fd);
    }
    
    rewind(fd);
    unsigned char in[40*1024];
   
    for(int i=0;i<count;i++){
        
        int ret = fread(in,1,len[i],fd);
       
        ret = LZ4_decompress_safe(in,out,len[i],51*1024);
        fwrite(out,ret,1,out_fd);
        printf("%d\n",ret);

    }
    fclose(fd);
    fclose(out_fd);
    }

    
}