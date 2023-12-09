#include <stdlib.h>

#include "structs.h"
#include "const.h"
#include "util.h"

#ifndef AES_H
#define AES_H



void computeroundkey128(RoundKey * input, RoundKey * output);
void computekeyschedule128(KeySchedule *);
void computekeyschedule192(KeySchedule *);
void computekeyschedule256(KeySchedule *);

void computekeyschedule(KeySchedule * keySchedule);

void subbytes(Block *);
void shiftrows(Block *);
void mixcolumn(byte *);
void mixcolumns(Block *);
void addroundkey(Block *, RoundKey *);

void encryptblock(Block *, KeySchedule *);

void aesECBencrypt(byte * message, byte * key);
void aesCBCencrypt(byte * message, byte * key, byte * iv);


#endif
