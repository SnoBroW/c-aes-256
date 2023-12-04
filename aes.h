#ifndef AES_H
#define AES_H

unsigned char **getschedule(unsigned char *key);

void subbytes(unsigned char ***block, int blocks);
void shiftrows(unsigned char ***block, int blocks);
void mixcolumn(unsigned char *column);
void mixcolumns(unsigned char ***block, int blocks);
void addroundkey(unsigned char ***block, int blocks, unsigned char *key);

void encryptround(unsigned char *data, unsigned char *key);


#endif
