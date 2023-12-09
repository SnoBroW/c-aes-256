#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"


int getdatalength(byte * _string){
    return (strlen((char *) _string));
}

int getblockcount(byte * _string){
    return (getdatalength(_string) / 16 + 1);
}

int getkeysize(byte * _string){
    return (getdatalength(_string) * 8);
}

void printmessage(Message * message) {
    Block * current = message->first;
    while(current != NULL) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%02x", current->data[i][j]);
            }
        }
        current = current->next;
    }
}

void printroundkey(RoundKey * roundKey) {
    for (int i = 0; i < roundKey->wordcount; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%02x", roundKey->value[i][j]);
        }
    }
}

void printkeyschedule(KeySchedule * keySchedule) {
    for (int i = 0; i < keySchedule->rounds; ++i) {
        printroundkey(keySchedule->roundkeys[i]);
        printf("\n");
    }
}

Message * newemptymessage(int size) {
    Message * message = malloc(sizeof(Message));
    message->size = size;
    message->first = NULL;
    message->last = NULL;
    return message;
}

Block * newblock() {
    Block * block = malloc(sizeof(Block));
    block->next = NULL;
    block->prev = NULL;
    block->data = malloc(4 * sizeof(byte *));
    for (int i = 0; i < 4; ++i) {
        block->data[i] = malloc(4 * sizeof(byte));
    }
    return block;
}

void appendblock(Message * message, Block * toadd) {
    if(message->first == NULL) {
        message->first = toadd;
    } else {
        Block * oldlast = message->last;
        oldlast->next = toadd;
        toadd->prev = oldlast;
    }
    message->last = toadd;
}

Message * createmessage(byte *_string) {
    int counter = 0;
    int len = getdatalength(_string);
    int blocks = getblockcount(_string);
    Block * block = NULL;

    Message * message = newemptymessage(blocks);
    message->size = blocks;

    for (int i = 0; i < blocks; i++) {
        block = newblock();
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                block->data[j][k] = counter < len ? _string[counter] : 16 - (len % 16);
                block->id = i;
                counter++;
            }
        }
        appendblock(message, block);
    }

    return message;
}

RoundKey * newroundkey(int round, int keysize) {
    RoundKey * roundKey = malloc(sizeof(RoundKey));
    roundKey->wordcount = keysize / 32;
    roundKey->round = round;

    roundKey->value = malloc(roundKey->wordcount * sizeof(byte *));
    for (int i = 0; i < roundKey->wordcount; ++i) {
        roundKey->value[i] = malloc(4 * sizeof(byte));
    }

    return roundKey;
}

KeySchedule * newemptykeyschedule(int keysize) {
    KeySchedule * keySchedule = malloc(sizeof(KeySchedule));
    keySchedule->keysize = keysize;
    keySchedule->rounds = ((keysize / 64) - 1) * 2 + 9;
    keySchedule->roundkeys = malloc(keySchedule->rounds * sizeof(RoundKey *));
    return keySchedule;
}

KeySchedule * createkeyschedule(byte * _string) {
    int keysize = getkeysize(_string);

    KeySchedule * keySchedule = newemptykeyschedule(keysize);
    keySchedule->roundkeys[0] = newroundkey(0, keysize);

    for (int i = 0; i < keySchedule->roundkeys[0]->wordcount; i++) {
        for (int j = 0; j < 4; j++) {
            keySchedule->roundkeys[0]->value[i][j] = _string[i * 4 + j];
        }
    }

    for (int i = 1; i < keySchedule->rounds; ++i) {
        keySchedule->roundkeys[i] = newroundkey(i, 128);
    }

    return keySchedule;
}

void freemessage(Message * message) {
    Block * current = message->first;
    while(current != NULL) {
        for (int i = 0; i < 4; ++i) {
            free(current->data[i]);
        }
        free(current->data);
        if(current->next != NULL) {
            current = current->next;
            free(current->prev);
        } else {
            free(current);
            current = NULL;
        }
    }
    free(message);
}

void freekeyschedule(KeySchedule * keySchedule){
    for (int i = 0; i < keySchedule->rounds; ++i) {
        for (int j = 0; j < keySchedule->roundkeys[i]->wordcount; ++j) {
            free(keySchedule->roundkeys[i]->value[j]);
        }
        free(keySchedule->roundkeys[i]->value);
        free(keySchedule->roundkeys[i]);
    }
    free(keySchedule->roundkeys);
    free(keySchedule);
}

void freestring(byte * _string) {
    free(_string);
}