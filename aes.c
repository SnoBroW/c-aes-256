#include "aes.h"

typedef unsigned char bool;


void computeroundkey128(RoundKey * input, RoundKey * output) {

    byte ** inkey = input->value;
    byte ** outkey = output->value;
    int atround = input->round;
    int wordcount = input->wordcount;


    for (int i = 0; i < 4; ++i) {
        outkey[0][i] = sbox[inkey[wordcount - 1][(i + 1) % 4] / 16][inkey[wordcount - 1][(i + 1) % 4] % 16] ^ ((i == 0) ? rcon[atround] : 0x00);
    }

    for (int i = 0; i < 4; i++) {
        outkey[0][i] ^= inkey[input->wordcount - 4][i];
    }

    for (int i = 1; i < wordcount; i++) {
        for (int j = 0; j < 4; j++) {
            outkey[i][j] = inkey[input->wordcount - 4 + i][j] ^ outkey[i - 1][j];
        }
    }
}

void computekeyschedule128(KeySchedule * keySchedule) {
    for (int i = 1; i < keySchedule->rounds; ++i) {
        computeroundkey128(keySchedule->roundkeys[i - 1], keySchedule->roundkeys[i]);
    }
}

void computekeyschedule192(KeySchedule * keySchedule) {
    int totalwordcount = keySchedule->rounds * 4;
    int keywordcount = keySchedule->keysize / 32;

    Word wordlist[totalwordcount];
    Word temp;

    for (int i = 0; i < keywordcount; ++i) {
        for (int j = 0; j < 4; ++j) {
            wordlist[i].value[j] = keySchedule->roundkeys[0]->value[i][j];
            temp.value[j] = wordlist[i].value[j];
        }
    }

    for (int i = keywordcount; i < totalwordcount; ++i) {
        if(!(i % 6)) {
            for (int j = 0; j < 4; ++j) {
                temp.value[j] = sbox[wordlist[i - 1].value[(j + 1) % 4] / 16][wordlist[i - 1].value[(j + 1) % 4] % 16] ^ ((j == 0) ? rcon[i / 6 - 1] : 0x00);
            }
        } else {
            for (int j = 0; j < 4; ++j) {
                temp.value[j] = wordlist[i - 1].value[j];
            }
        }
        for (int j = 0; j < 4; ++j) {
            wordlist[i].value[j] = temp.value[j] ^ wordlist[i - 6].value[j];
        }
    }

    for (int i = 1; i < keySchedule->rounds; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                keySchedule->roundkeys[i]->value[j][k] = wordlist[i * 4 + j].value[k];
            }
        }
    }
}


void computekeyschedule256(KeySchedule * keySchedule) {
    int totalwordcount = keySchedule->rounds * 4;
    int keywordcount = keySchedule->keysize / 32;

    Word wordlist[totalwordcount];
    Word temp;

    for (int i = 0; i < keywordcount; ++i) {
        for (int j = 0; j < 4; ++j) {
            wordlist[i].value[j] = keySchedule->roundkeys[0]->value[i][j];
            temp.value[j] = wordlist[i].value[j];
        }
    }

    for (int i = keywordcount; i < totalwordcount; ++i) {
        if(!(i % 8)) {
            for (int j = 0; j < 4; ++j) {
                temp.value[j] = sbox[wordlist[i - 1].value[(j + 1) % 4] / 16][wordlist[i - 1].value[(j + 1) % 4] % 16] ^ ((j == 0) ? rcon[i / 8 - 1] : 0x00);
            }
        } else if(!(i % 4)) {
            for (int j = 0; j < 4; ++j) {
                temp.value[j] = sbox[wordlist[i - 1].value[j] / 16][wordlist[i - 1].value[j] % 16];
            }
        } else {
            for (int j = 0; j < 4; ++j) {
                temp.value[j] = wordlist[i - 1].value[j];
            }
        }
        for (int j = 0; j < 4; ++j) {
            wordlist[i].value[j] = temp.value[j] ^ wordlist[i - 8].value[j];
        }
    }

    for (int i = 1; i < keySchedule->rounds; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                keySchedule->roundkeys[i]->value[j][k] = wordlist[i * 4 + j].value[k];
            }
        }
    }
}

void computekeyschedule(KeySchedule * keySchedule) {
    switch(keySchedule->keysize) {
        case 128:
            computekeyschedule128(keySchedule);
            break;
        case 192:
            computekeyschedule192(keySchedule);
            break;
        case 256:
            computekeyschedule256(keySchedule);
            break;
        default:
            break;
    }
}

void subbytes(Block * block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            block->data[i][j] = sbox[block->data[i][j] / 16][block->data[i][j] % 16];
        }
    }
}

void shiftrows(Block * block) {
    byte temp[4][4];
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            temp[j][k] = block->data[(k + j) % 4][k];
        }
    }
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            block->data[j][k] = temp[j][k];
        }
    }
}

void mixcolumn(byte *column) {
    byte copy[4];
    byte shifted[4];
    bool isTooBig;
    for (int i = 0; i < 4; i++) {
        copy[i] = column[i];
        isTooBig = column[i] >> 7;
        shifted[i] = column[i] << 1;
        if (isTooBig) {
            shifted[i] ^= 0x1B;
        }
    }
    for (int i = 0; i < 4; i++) {
        column[i] = 0;
        for (int j = 0; j < 4; j++) {
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

void mixcolumns(Block * block) {
    unsigned char temp[4];
    for (int j = 0; j < 4; j++) {
        for(int k = 0; k < 4; k++) {
            temp[k] = block->data[j][k];
        }
        mixcolumn(temp);
        for(int k = 0; k < 4; k++) {
            block->data[j][k] = temp[k];
        }
    }
}

void addroundkey(Block * block, RoundKey * roundKey) {
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            block->data[j][k] ^= roundKey->value[j][k];
        }
    }
}

void encryptblock(Block * block, KeySchedule * keySchedule) {

    addroundkey(block, keySchedule->roundkeys[0]);

    for (int round = 1; round < keySchedule->rounds; ++round) {
        subbytes(block);
        shiftrows(block);
        if(round != keySchedule->rounds - 1) {
            mixcolumns(block);
        }
        addroundkey(block, keySchedule->roundkeys[round]);
    }
}

void aesECBencrypt(unsigned char * data, unsigned char * keystring) {

    Message * message = createmessage(data);

    KeySchedule * keyschedule = createkeyschedule(keystring);
    computekeyschedule(keyschedule);

    Block * current = message->first;

    while(current != NULL) {
        encryptblock(current, keyschedule);
        current = current->next;
    }

    applytoinputptr(data, message);

    freekeyschedule(keyschedule);
    freemessage(message);
}


void aesCBCencrypt(byte * data, byte * keystring, byte * iv) {
    Message * message = createmessage(data);

    KeySchedule * keyschedule = createkeyschedule(keystring);
    computekeyschedule(keyschedule);

    Block * ivblock = getblock(iv);
    Block * current = message->first;

    while(current != NULL) {
        xorblocks(current, current->prev == NULL ? ivblock : current->prev);
        encryptblock(current, keyschedule);
        current = current->next;
    }

    applytoinputptr(data, message);

    freekeyschedule(keyschedule);
    freemessage(message);
    freeblock(ivblock);
}