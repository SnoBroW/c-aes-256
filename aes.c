#include "const.h"
#include "util.h"

typedef unsigned char bool;

void computenextkey(unsigned char ** key, int wordcount, int atround) {
    unsigned char lastword[4] = {
            sbox[key[wordcount - 1][1] / 16][key[wordcount - 1][1] % 16] ^ rcon[atround],
            sbox[key[wordcount - 1][2] / 16][key[wordcount - 1][2] % 16],
            sbox[key[wordcount - 1][3] / 16][key[wordcount - 1][3] % 16],
            sbox[key[wordcount - 1][0] / 16][key[wordcount - 1][0] % 16]
    };
    for (int i = 0; i < 4; i++) {
        key[0][i] ^= lastword[i];
    }
    for (int i = 1; i < wordcount; i++) {
        for (int j = 0; j < 4; j++) {
            key[i][j] ^= key[i - 1][j];
        }
    }
}

void subbytes(unsigned char *** block, int blocks) {
    for (unsigned char i = 0; i < blocks; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            for (unsigned char k = 0; k < 4; k++) {
                block[i][j][k] = sbox[block[i][j][k] / 16][block[i][j][k] % 16];
            }
        }
    }
}

//void shiftrows(unsigned char *** block, int blocks) {
//    unsigned char temp[4];
//    for (unsigned char i = 0; i < blocks; i++) {
//        for (unsigned char j = 0; j < 4; j++) {
//            for (unsigned char k = 0; k < 4; k++) {
//                temp[k] = block[i][(k + j) % 4][j];
//            }
//            for (int k = 0; k < 4; k++) {
//                block[i][j][k] = temp[k];
//            }
//        }
//    }
//}

void shiftrows(unsigned char *** block, int blocks) {
    unsigned char temp[4][4];
    for (int i = 0; i < blocks; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                temp[j][k] = block[i][(k + j) % 4][k];
            }
        }
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                block[i][j][k] = temp[j][k];
            }
        }
    }
}

void mixcolumn(unsigned char *column) {
    unsigned char copy[4];
    unsigned char shifted[4];
    bool isTooBig;
    for (unsigned char i = 0; i < 4; i++) {
        copy[i] = column[i];
        isTooBig = column[i] >> 7;
        shifted[i] = column[i] << 1;
        if (isTooBig) {
            shifted[i] ^= 0x1B;
        }
    }
    for (unsigned char i = 0; i < 4; i++) {
        column[i] = 0;
        for (unsigned char j = 0; j < 4; j++) {
            switch (mds[i][j]) {
                case 1:
                    column[i] ^= copy[j];
                    break;
                case 2:
                    column[i] ^= shifted[j];
                    break;
                case 3:
                    column[i] ^= (copy[j] ^ shifted[j]);
                    break;
            }
        }
    }
}

void mixcolumns(unsigned char *** block, int blocks) {
    unsigned char temp[4];
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                temp[k] = block[i][j][k];
            }
            mixcolumn(temp);
            for(int k = 0; k < 4; k++) {
                block[i][j][k] = temp[k];
            }
        }
    }
}

void addroundkey(unsigned char *** block, int blocks, unsigned char ** key, int atround) {
    computenextkey(key, 4, atround);
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            block[0][j][k] ^= key[j][k];
        }
    }
}

void xor(byte *** block, int id, byte ** toxor){
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            block[id][j][k] ^= toxor[j][k];
        }
    }
}

void encryptround(byte *** block, int blocks, byte ** key, int atround) {

    if(atround == 0) {
        xor(block, 0, key);
    }

    subbytes(block, blocks);

    shiftrows(block, blocks);

    if(atround != 9) {
        mixcolumns(block, blocks);
    }

    addroundkey(block, blocks, key, atround);
}



void aesencrypt(unsigned char * data, unsigned char * keystring) {

    int blocks = getblockcount(data);
    unsigned char *** block = getblock(data, blocks);

    int wordcount = getwordcount(keystring);
    unsigned char ** key = getkey(keystring, wordcount);



    for (int i = 0; i < 10; ++i) {
        encryptround(block, blocks, key, i);
    }

    printkey(block[0], 4);

    freeblocks(block, blocks);
    freekey(key, wordcount);
    freestring(data);
}