#ifndef LOG_COMPRESS_H
#define LOG_COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lz4hc.h"


extern _Bool init(uint32_t size);

/*
* @brief compress log
* @param[in] src     - the address of log need compress
* @param[in] src_len - the length of the log 
*/
extern void LogCompress(const unsigned char* src,int src_len);


extern void release();

static void CompressAndWrite();
#endif