#include "io.h"
#include "aes.h"
#include "structs.h"

int main(int argc, char *argv[]) {

    byte * input = readstdin(2048);
    int inputlength = getdatalength(input);
    Options * options = parseargs(argc, argv);

    switch (options->mode) {
        case ECB:
            aesECBencrypt(input, options->key);
            break;
        case CBC:
            aesCBCencrypt(input, options->key, options->iv);
            free(options->iv);
            break;
        default:
            printf("bien joué! le flag est ”HOW_DID_YOU_GET_HERE”");
    }

    for (int i = 0; i < inputlength + 16 - (inputlength % 16); ++i) {
        printf(options->output == HEX ? "%02x" : "%c", input[i]);
    }

    free(input);

    free(options->key);
    free(options);

    return 0;
}
