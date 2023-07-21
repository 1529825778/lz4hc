#include "logcompress.h"

/*compress_size is from PRE_COMPRESS_SIZE to PRE_COMPRESS_SIZE+1024*/
#define PRE_COMPRESS_SIZE 50*1024;

static uint32_t count =0;


/*the array of compress size written into the end of the log for decompress*/
static uint32_t CompressSize[4096];

static FILE* CompressFile;

typedef struct compress_worker_t{
    uint32_t total_size;
    uint32_t used_size;
    unsigned char* begin_ptr;
    unsigned char* work_ptr;
}compress_worker_t;

static compress_worker_t* compress_worker;

//init :create file
_Bool init(){

    time_t timep;
    struct tm *t;
    time(&timep);
    t = localtime(&timep);
    char filename[20];
    sprintf(filename,"%d_%d_%d_%d_%d_%d.log",1900+t->tm_year,1+t->tm_mon,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    CompressFile = fopen(filename,"ab+");
    /*
    CompressFile = fopen(filename,"ab+");
    */
    if(CompressFile ==NULL){
        return 0;
    }
    compress_worker =(compress_worker_t*)malloc(sizeof(compress_worker_t));
    memset(compress_worker,0x00,sizeof(compress_worker_t));
    if(compress_worker==NULL){
        return 0;
    }

    compress_worker->total_size = PRE_COMPRESS_SIZE +1024;
    compress_worker->begin_ptr = (unsigned char*)malloc(compress_worker->total_size);
    memset(compress_worker->begin_ptr,0x00,compress_worker->total_size);
    compress_worker->work_ptr = compress_worker->begin_ptr;
    compress_worker->used_size = 0;

    return 1;
}


void CompressAndWrite(){
    unsigned char out[compress_worker->used_size];
    int out_len;
    out_len = LZ4_compress_HC(compress_worker->begin_ptr,out,compress_worker->used_size,LZ4_compressBound(compress_worker->used_size),12);
    if(out_len>0){
        CompressSize[count++] = out_len;
        compress_worker->work_ptr = compress_worker->begin_ptr;
        compress_worker->used_size=0;
        fwrite(out,out_len,1,CompressFile);
        
    }
    else printf("compress fail!\n");
    
}



void LogCompress(const unsigned char* src,int src_len){
    /*compress*/
   
    memcpy(compress_worker->work_ptr,src,src_len);
    compress_worker->work_ptr+=src_len;
    compress_worker->used_size+=src_len;
    if(compress_worker->used_size>=compress_worker->total_size-1024){
        CompressAndWrite();
    }
}

/*save left log in compress_worker and release memory*/
void release(){
    CompressAndWrite();
    for(int i=0;i<count;i++){
        fwrite(&CompressSize[i],sizeof(short),1,CompressFile);
    }
    fwrite(&count,sizeof(short),1,CompressFile);
    free(compress_worker->work_ptr);
    free(compress_worker);
    fclose(CompressFile);
    printf("count:%d\n",count);

}

/*test*/

// void main(){
//     FILE* file = fopen("rtk_0718-1.log","r");
//     char buf[2048];
   
//     if(!init()){
//         printf("init fail!\n");
//     }

//     while (fgets(buf, sizeof(buf), file) != NULL)
//     {
//         LogCompress(buf,strlen(buf));
//     }
    
//      release();
// }


