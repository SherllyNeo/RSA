#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "rsa.c"

#define MAX_MESSAGE_LENGTH 1024


void initNewKeys(char* filepath);

void encrypt(char* text, char* filepath, char returnBuffer[MAX_MESSAGE_LENGTH]);

void decrypt(char cipherText[MAX_MESSAGE_LENGTH], char* filepath, char returnString[MAX_MESSAGE_LENGTH]);
