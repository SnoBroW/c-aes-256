#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "structs.h"

#ifndef C_AES_256_IO_H
#define C_AES_256_IO_H


unsigned char * readstdin(int size);

void printmessage(Message *);
void printroundkey(RoundKey *);
void printkeyschedule(KeySchedule *);

Options * parseargs(int, char **);

#endif
