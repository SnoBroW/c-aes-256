#ifndef AES_H
#define AES_H

#include "structs.h"

void computeroundkey128(RoundKey * input, RoundKey * output);
void computekeyschedule128(KeySchedule *);
void computekeyschedule192(KeySchedule *);
void computekeyschedule256(KeySchedule *);

void computekeyschedule(KeySchedule * keySchedule);

void subbytes(Message *);
void shiftrows(Message *);
void mixcolumn(byte *);
void mixcolumns(Message *);
void addroundkey(Message *, RoundKey *);

void encryptround(Message *, KeySchedule *, int round);

void aesencrypt(unsigned char * message, unsigned char * key);


#endif
