 
 ###
 该模块能将49.4MB的测试文件压缩至9.8MB
 ### how to use

step1: #include "logcompress.h"

step2: 调用init(uint32_t size),创建输出文件并设置每次压缩的大小。

step3: 调用LogCompress(unsigned char* src,int srclen);第一个参数为要压缩的日志，第二个参数为压缩的日志长度，当累计长度小于50kb时，会先存储，直到长度大于50kb时进行压缩并写入文件。

step4: 在程序执行完毕后，调用release();压缩内存中残留的不足50kb的日志并写入文件。

### how to decompress
使用可执行文件decompress,命令：./decompress 压缩后的日志名。

