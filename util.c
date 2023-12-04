#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int getdatalength(unsigned char * _string){
    return (strlen((char *) _string));
}

int getblockcount(unsigned char * _string){
    return (getdatalength(_string) / 16 + 1);
}

void printblocks(unsigned char ***block, int blocks) {
    printf("-----------\n");
    for (unsigned char i = 0; i < blocks; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            for (unsigned char k = 0; k < 4; k++) {
                if (block[i][j][k] != '\0') {
                    printf("%02x ", block[i][j][k]);
                } else {
                    printf("  ");
                }
                if (k == 3) {
                    printf("\n");
                }
            }
        }
        printf("-----------\n");
    }
}


unsigned char ***getblock(unsigned char *_string, int blocks) {
    int counter = 0;
    int len = getdatalength(_string);

    unsigned char *** block = malloc(blocks * sizeof(unsigned char **));
    for (unsigned char i = 0; i < blocks; i++) {
        block[i] = malloc(4 * sizeof(unsigned char *));
        for (unsigned char j = 0; j < 4; j++) {
            block[i][j] = malloc(4 * sizeof(unsigned char));
        }
    }

    for (unsigned char i = 0; i < blocks; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            for (unsigned char k = 0; k < 4; k++) {
                block[i][j][k] = counter < len ? _string[counter] : 16 - (len % 16);
                counter++;
            }
        }
    }

    return block;
}

void freeblocks(unsigned char ***block, int blocks) {
    for (unsigned char i = 0; i < blocks; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            free(block[i][j]);
        }
        free(block[i]);
    }
    free(block);
}