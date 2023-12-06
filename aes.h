#ifndef AES_H
#define AES_H



void computenextkey(unsigned char **, int, int);

void subbytes(unsigned char ***, int);
void shiftrows(unsigned char ***, int);
void mixcolumn(unsigned char *);
void mixcolumns(unsigned char ***, int );
void addroundkey(unsigned char ***, int, unsigned char *);

void encryptround(unsigned char ***, int, unsigned char *);

void aesencrypt(unsigned char *, unsigned char *, unsigned char *);


#endif
