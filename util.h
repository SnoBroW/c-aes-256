#ifndef AES_UTIL_H
#define AES_UTIL_H

#include "structs.h"

int getdatalength(byte *);
int getblockcount(byte *);
int getkeysize(byte *);

void printmessage(Message *);
void printroundkey(RoundKey *);
void printkeyschedule(KeySchedule *);

Message * newemptymessage(int);
Block * newblock();
void appendblock(Message *, Block *);
Message * newemptymessage();
Message * createmessage(byte *);

RoundKey * newroundkey(int round, int keysize);
KeySchedule * newemptykeyschedule(int);
KeySchedule * createkeyschedule(byte *);

void freemessage(Message *);
void freekeyschedule(KeySchedule *);
void freestring(byte *);

#endif
