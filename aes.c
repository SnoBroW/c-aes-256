#include "const.h"
#include "util.h"

typedef unsigned char bool;


unsigned char ** getschedule(unsigned char *key) {
    return 0;
}



void subbytes(unsigned char ***block, int blocks) {
    for (unsigned char i = 0; i < blocks; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            for (unsigned char k = 0; k < 4; k++) {
                block[i][j][k] = sbox[block[i][j][k] / 16][block[i][j][k] % 16];
            }
        }
    }
}

void shiftrows(unsigned char ***block, int blocks) {
    unsigned char temp[4];
    for (unsigned char i = 0; i < blocks; i++) {
        for (unsigned char j = 0; j < 4; j++) {
            for (unsigned char k = 0; k < 4; k++) {
                temp[k] = block[i][j][(k + j) % 4];
            }
            for (int k = 0; k < 4; k++) {
                block[i][j][k] = temp[k];
            }
            // (k + j) % 4 -> source byte
            // (k - j + 4) % 4 -> destination byte
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

void mixcolumns(unsigned char ***block, int blocks) {
    unsigned char temp[4];
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                temp[k] = block[i][k][j];
            }
            mixcolumn(temp);
            for(int k = 0; k < 4; k++) {
                block[i][k][j] = temp[k];
            }
        }
    }
}

void addroundkey(unsigned char ***block, int blocks, unsigned char *key) {

}

void encryptround(unsigned char *data, unsigned char *key) {

    int blocks = getblockcount(data);
    unsigned char ***block = getblock(data, blocks);

    printblocks(block, blocks);
    subbytes(block, blocks);
    shiftrows(block, blocks);
    mixcolumns(block, blocks);
    printblocks(block, blocks);

    freeblocks(block, blocks);
}


