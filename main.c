#include <stdio.h>
#include <stdlib.h>

#include "aes.h"

int main(int argc, char *argv[]) {

    unsigned char *string = malloc(1024 * sizeof(unsigned char));
    fgets((char *) string, 1024, stdin);

    unsigned char *key = (unsigned char *) "aledaledaledaled";
    aesencrypt(string, key);


    return 0;
}
