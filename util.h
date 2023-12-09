#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

#ifndef AES_UTIL_H
#define AES_UTIL_H

int getdatalength(byte *);
int getblockcount(byte *);
int getkeysize(byte *);

void printmessage(Message * message);
void printroundkey(RoundKey * roundKey);
void printkeyschedule(KeySchedule * keySchedule);

Block * newblock();
Block * getblock(byte *);

void appendblock(Message *, Block *);
void xorblocks(Block *, Block *);

Message * newemptymessage(int size);
Message * createmessage(byte *);

RoundKey * newroundkey(int round, int keysize);
KeySchedule * newemptykeyschedule(int);
KeySchedule * createkeyschedule(byte *);

void applytoinputptr(byte * input, Message * message);

void freemessage(Message *);
void freeblock(Block *);
void freekeyschedule(KeySchedule *);
void freestring(byte *);

#endif
