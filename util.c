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

int getwordcount(byte * _string){
    return (getdatalength(_string) / 4);
}

void printblocks(byte ***block, int blocks) {
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                printf("%02x ", block[i][j][k]);
            }
            printf("\n");
        }
    }
}

void printkey(byte ** key, int wordcount) {
    for (int i = 0; i < wordcount; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%02x", key[i][j]);
        }
    }
}

Block * newblock() {
    Block * block = malloc(sizeof(Block));
    block->next = NULL;
    block->prev = NULL;
    block->data = malloc(4 * sizeof(byte));
    for (int i = 0; i < 4; ++i) {
        block->data[i] = malloc(sizeof(byte));
    }
    return block;
}

Message * newemptymessage(int size) {
    Message * message = malloc(sizeof(Message));
    message->size = size;
    message->first = NULL;
    message->last = NULL;
    return message;
}

byte *** getblock(byte *_string, int blocks) {
    int counter = 0;
    int len = getdatalength(_string);

    byte *** block = malloc(blocks * sizeof(byte **));

    for (int i = 0; i < blocks; i++) {
        block[i] = malloc(4 * sizeof(byte *));
        for (int j = 0; j < 4; j++) {
            block[i][j] = malloc(4 * sizeof(byte));
        }
    }

    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                block[i][j][k] = counter < len ? _string[counter] : 16 - (len % 16);
                counter++;
            }
        }
    }

    return block;
}

byte ** getkey(byte * _string, int wordcount) {
    int counter = 0;

    byte ** key = malloc(wordcount * sizeof(byte *));

    for (int i = 0; i < wordcount; i++) {
        key[i] = malloc(4 * sizeof(byte));
    }

    for (int i = 0; i < wordcount; i++) {
        for (int j = 0; j < 4; j++) {
            key[i][j] = _string[counter++];
        }
    }

    return key;
}

void freeblocks(byte ***block, int blocks) {
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < 4; j++) {
            free(block[i][j]);
        }
        free(block[i]);
    }
    free(block);
}

void freekey(byte ** key, int wordcount){
    for (int i = 0; i < wordcount; i++){
            free(key[i]);
    }
    free(key);
}

void freestring(byte * _string) {
    free(_string);
}