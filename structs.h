
#ifndef C_AES_256_STRUCTS_H
#define C_AES_256_STRUCTS_H

typedef unsigned char byte;

typedef struct block {
    byte ** data;
    byte id;
    struct block * next;
    struct block * prev;
} Block;

typedef struct message {
    Block * first;
    Block * last;
    int size;
} Message;

typedef struct roundkey {
    byte ** value;
    unsigned int round;
} RoundKey;

typedef struct keyschedule {
    RoundKey * roundkeys;
    int wordcount;
} KeySchedule;


#endif
