#include "logcompress.h"

void main(){
    FILE* file = fopen("rtk_0718-1.log","r");
    char buf[2048];
   
    if(!init()){
        printf("init fail!\n");
    }

    /*read per line of log to compress*/

    while (fgets(buf, sizeof(buf), file) != NULL)
    {
        LogCompress(buf,strlen(buf));
    }
    
     release();
}
// gcc logcompress.c lz4.c lz4hc.c test.c -o test
