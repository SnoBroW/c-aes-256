#ifndef AES_UTIL_H
#define AES_UTIL_H

int getdatalength(unsigned char * _string);
int getblockcount(unsigned char * _string);

void printblocks(unsigned char ***block, int blocks);

unsigned char ***getblock(unsigned char *_string, int blocks);
void freeblocks(unsigned char ***block, int blocks);

#endif
