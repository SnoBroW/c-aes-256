#ifndef AES_UTIL_H
#define AES_UTIL_H

#include "structs.h"

int getdatalength(byte *);
int getblockcount(byte *);
int getwordcount(byte *);

void printblocks(byte ***, int);
void printkey(byte **, int);

Block * newblock();
Message * newemptymessage();

byte *** getblock(byte *, int);
byte ** getkey(byte *, int);

void freeblocks(byte ***, int);
void freekey(byte **, int);
void freestring(byte *);

#endif
