#include "io.h"


unsigned char * readstdin(int size) {
    // a changer rapidement, je remplacerai par une lecture bloc par bloc de stdin
    unsigned char * string = malloc(size * sizeof(unsigned char));
    fgets((char *) string, size, stdin);
    int len = strlen((char *) string);
    if (len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
    return string;
}


void printusage() {
    printf("usage:\n\n"
           "    -m\n"
           "        sets the AES mode to use\n"
           "        available modes: 1 - ECB, 2 - CBC\n"
           "    -k\n"
           "        sets the key to use for encryption\n"
           "        available key lengths (bytes): 16, 24, 32\n"
           "    -i\n"
           "        sets the IV to use for encryption\n"
           "        only used in CBC mode\n"
           "    -H\n"
           "        will output the ciphertext as hex\n"
           "    -h\n"
           "        shows this message\n");
}


Options * parseargs(int argc, char * argv[]) {
    Options * options = malloc(sizeof(Options));

    options->mode = -1;
    options->output = RAW;
    options->keysize = -1;

    options->key = NULL;
    options->iv = NULL;

    int opt;
    int len;

    if(argc == 1) {
        printf("not enough arguments supplied\n\n");
        printusage();
        exit(-1);
    }

    while((opt = getopt(argc, argv, "hm:k:i:H")) != -1)
    {
        switch(opt)
        {
            case 'm':
                if(strcasecmp(optarg, "ECB") == 0) {
                    options->mode = ECB;
                } else if(strcasecmp(optarg, "CBC") == 0) {
                    options->mode = CBC;
                } else {
                    printf("invalid mode \"%s\"\n", optarg);
                    exit(-1);
                }
                break;
            case 'k':
                len = strlen(optarg);
                if(len == 16 || len == 24 || len == 32) {
                    options->key = malloc((len + 1) * sizeof(byte));
                    memcpy(options->key, optarg, len);
                    options->keysize = len * 8;
                } else {
                    printf("invalid key length\n");
                    exit(-1);
                }
                break;
            case 'i':
                len = strlen(optarg);
                if(options->mode != CBC && options->mode != 0) {
                    printf("no need for IV in ECB\n");
                    exit(-1);
                } else if(len == 16) {
                    options->iv = malloc((len + 1) * sizeof(byte));
                    memcpy(options->iv, optarg, len);
                } else {
                    printf("invalid IV length\n");
                    exit(-1);
                }
                break;
            case 'H':
                options->output = HEX;
                break;
            case 'h':
                printusage();
                exit(0);
            case ':':
                printf("option -%c requires an argument\n", optopt);
                break;
            case '?':
                printf("unknown option: -%c\n", optopt);
                break;
        }
    }

    if(options->mode == -1) {
        printf("no mode selected\n");
        exit(-1);
    }

    if(options->keysize == -1) {
        printf("enter a key:\n");
        byte * key = readstdin(512);
        len = strlen((char *) key);
        if(len == 16 || len == 24 || len == 32) {
            options->key = malloc((len + 1) * sizeof(byte));
            memcpy(options->key, key, len);
            options->keysize = len * 8;
            free(key);
        } else {
            printf("invalid key length\n");
            exit(-1);
        }
    }

    if(options->mode == CBC && options->iv == NULL) {
        printf("enter an IV:\n");
        byte * iv = readstdin(512);
        len = strlen((char *) iv);
        if(len == 16) {
            options->iv = malloc(16 * sizeof(byte));
            memcpy(options->iv, iv, 16);
            free(iv);
        } else {
            printf("invalid IV length\n");
            exit(-1);
        }
    }

    return options;
}