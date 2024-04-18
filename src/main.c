#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "rsa.h"

#define EXT_BAD_PARAMS 1

char help[] = "\nMoriarty's RSA Implimentation \n---------------------- \n\n \
-h or --help to view help\n \
-i or --init to make new public and private keys, you can specify -k or --key_file_path to say where the key files should be, it will defeault to ./keys\n \
-e or --encrypt to encrypt: rsa -e -t \"sample message\" -k \"path\\to\\publickey\"\n \
-d or --decrypt to decrypt: rsa -d -t \"19229,338383\" -k \"path\\to\\privateckey\"\n \
-k or --key_file_path is used to specify the path to either a public or private keyfile\n \
-t or --text is used to specify the plaintext or ciphertext\n \
";


int main(int argc, char* argv[]) {


    /*  Arg parsing */
    char* key_file_path = NULL;
    char* text = NULL;
    bool encryptText = true;
    bool decryptText = false;

    if (argc < 2) {
        printf("%s\n",help);
    }

    for (int i=0; i<argc; i++) {
        char* current_arg = argv[i];
        if (!strcmp("-h",current_arg) || !strcmp("--help",current_arg)) {
            printf("\n%s\n",help);
            return 0;
        }
        else if (!strcmp("-i",current_arg) || !strcmp("--init",current_arg)) {
            initNewKeys();
            printf("Initalised keys in ./keys directory\n");
            return 0;
        }
        else if (!strcmp("-e",current_arg) || !strcmp("--encrypt",current_arg)) {
            encryptText = true;
            decryptText = false;
        }
        else if (!strcmp("-d",current_arg) || !strcmp("--decrypt",current_arg)) {
            decryptText = true;
            encryptText = false;
        }
        else if (!strcmp("-k",current_arg) || !strcmp("--key_file_path",current_arg)) {
            key_file_path = argv[++i]; 
        }
        else if (!strcmp("-t",current_arg) || !strcmp("--text",current_arg)) {
            text = argv[++i]; 
        }

    }

    if (!key_file_path) {
        printf("must provide a keyfile path with -k or --key_file_path, this can be either a private or public key file\nTry -h or --help for help\n");
        exit(EXT_BAD_PARAMS);
    }
    if (!text) {
        printf("must provide ciphertext or plaintext with -t or --text\nTry -h or --help for help\n");
        exit(EXT_BAD_PARAMS);
    }

    if (encryptText) {
        printf("encrypting: %s\n",text);
        char encryptedBuffer[MAX_MESSAGE_LENGTH];
        memset(encryptedBuffer,'\0',sizeof(encryptedBuffer));
        encrypt(text, key_file_path,encryptedBuffer);
        printf("[+] done\n %s\n",encryptedBuffer);
    }
    else if (decryptText) {
        printf("decrypting %s\n",text);
        char decryptedBuffer[MAX_MESSAGE_LENGTH];
        memset(decryptedBuffer,'\0',sizeof(decryptedBuffer));
        decrypt(text, key_file_path,decryptedBuffer);
        printf("[+] done\n %s\n",decryptedBuffer);
    }
    else {
        printf("ERROR: please input correct parameters\n");
        printf("%s\n",help);
        exit(EXT_BAD_PARAMS);
    }




    return 0;
}
