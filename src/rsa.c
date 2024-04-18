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

#define MAX_MESSAGE_LENGTH 1024
#define MIN 10000
#define MAX 20000
#define GAP 5000

/* 
TODO: use gmp library to use abitrary size
 */

 
unsigned long int gcd(unsigned long int x, unsigned long int y) {
    while (y != 0) {
        unsigned long int temp = y;
        y = x % y;
        x = temp;
    }
    return x;
}

bool areCoprime(unsigned long int x, unsigned long int y) {
    return gcd(x, y) == 1;
}


bool isPrime(unsigned long int num) {
    if (num <= 1)
        return false;
    for (unsigned long int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

unsigned long int generateRandomPrime(unsigned long int lower, unsigned long int upper) {
    srand(time(NULL));

    unsigned long int num = (rand() % (upper - lower + 1)) + lower;

    if (num % 2 == 0)
        num++;

    while (!isPrime(num)) {
        num += 2; 
        if (num > upper) 
            num = lower + (num - upper - 1);
    }

    return num;
}

unsigned long int modExp(unsigned long int base, unsigned long int exponent, unsigned long int modulus) {
    unsigned long int result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

unsigned long int modInverse(unsigned long int a, unsigned long int m) {
    long int m0 = m, t, q;
    long int x0 = 0, x1 = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}


unsigned long int findPublicKey(unsigned long int phi_n) {
    unsigned long int public_key = 3; 
    if (public_key < phi_n && areCoprime(public_key, phi_n) && isPrime(public_key)) {
            return public_key;
    }
    else {
        for (unsigned long int i = 3; i < phi_n; i += 2) {
            if (i < phi_n && areCoprime(i, phi_n)) {
                return i;
            }
        }
    }
    return -1; 
}

unsigned long int transform(unsigned long int message, unsigned long int exponent, unsigned long int modulus) {
    return modExp(message, exponent, modulus);
}

void transformArray(int inputLen,unsigned long int input[MAX_MESSAGE_LENGTH], unsigned long int output[MAX_MESSAGE_LENGTH],unsigned long int exponent, unsigned long int modulus) {
    for (int i = 0; i < inputLen; i++) {
        unsigned long int trans = transform(input[i], exponent, modulus);
        output[i] = trans; 
    }
}

void printArray(int length, int unsigned long arr[MAX_MESSAGE_LENGTH]) {
    for (int i =0; i<length;i++) {
        printf("%c ",(char)arr[i]);
    }
    printf("\n");
}

void arrayToString(int length, int unsigned long arr[MAX_MESSAGE_LENGTH],char string[MAX_MESSAGE_LENGTH]) {
    for (int i =0; i<length;i++) {
        string[i] = (char)arr[i];
    }
}

void cipherarrayToString(const unsigned long int array[], int size, char *str) {
    strcpy(str, ""); // Clear the string
    char temp[20]; // Temporary buffer to hold each converted integer

    for (int i = 0; i < size; i++) {
        if (i < size-1) {
            sprintf(temp, "%ld,", array[i]); // Convert the current integer to a string
            strcat(str, temp); // Append the converted integer to the string
        }
        else {
            sprintf(temp, "%ld\n", array[i]); // Convert the current integer to a string
            strcat(str, temp); // Append the converted integer to the string
        }
    }

    // Remove the trailing comma
    if (strlen(str) > 0) {
        str[strlen(str) - 1] = '\0';
    }
}

void cipherstringToArray(const char *str, unsigned long int array[], int *size) {
    char *token;
    char copy[strlen(str) + 1];

    strcpy(copy, str); 
    *size = 0; 

    token = strtok(copy, ",");
    while (token != NULL && *size < MAX_MESSAGE_LENGTH) {
        array[(*size)++] = strtol(token, NULL, 10);
        token = strtok(NULL, ","); 
    }
}


void fileWrite(const char *filepath, const char *buffer)
{
    FILE *f = fopen(filepath, "w");
    if (f != NULL)
    {
        fputs(buffer, f);
        fclose(f);
    }
}


void fileRead(char * filepath, char buffer[MAX_MESSAGE_LENGTH]) {
    FILE *f = fopen(filepath, "rb");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    unsigned long length = ftell(f);
    rewind(f);

    if (length >= MAX_MESSAGE_LENGTH) {
        printf("File is too large.\n");
        fclose(f);
        return;
    }

    fread(buffer, sizeof(char), length, f);
    buffer[length] = '\0'; 

    fclose(f);

}


void initNewKeys(char* filepath) {
    unsigned long int p, q;
    p = generateRandomPrime(MIN, MAX);
    q = generateRandomPrime(MIN, MAX);
    while ((p-q) < GAP) {
        q = generateRandomPrime(MIN, MAX);
    }
    unsigned long int n = p * q;
    unsigned long int phi_n = (p - 1) * (q - 1);
    unsigned long int public_key_e = findPublicKey(phi_n);
    unsigned long int private_key_d = modInverse(public_key_e, phi_n);

    char pubBuffer[MAX_MESSAGE_LENGTH];
    memset(pubBuffer,'\0',MAX_MESSAGE_LENGTH*sizeof(char));
    sprintf(pubBuffer,"%li,%li\n", public_key_e,n);

    char privBuffer[MAX_MESSAGE_LENGTH];
    memset(privBuffer,'\0',MAX_MESSAGE_LENGTH*sizeof(char));
    sprintf(privBuffer,"%li,%li\n", private_key_d,n);


    /* file writing */

    if (filepath != NULL) {
        char pubPath[MAX];
        char privPath[MAX];
        mkdir(filepath, S_IRWXU);
        sprintf(pubPath,"%s/pubkey.pub",filepath);
        sprintf(privPath,"%s/privkey.priv",filepath);
        fileWrite(pubPath, pubBuffer);
        fileWrite(privPath, privBuffer);
    }
    else {
        mkdir("./keys", S_IRWXU);
        fileWrite("./keys/pubKey.pub", pubBuffer);
        fileWrite("./keys/privKey.priv", privBuffer);
    }
}


int countCommas(const char *str) {
    int count = 0;
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        if (str[i] == ',') {
            count++;
        }
    }

    return count;
}

void encrypt(char* text, char* filepath, char returnBuffer[MAX_MESSAGE_LENGTH]) {
    /* read key and n */
    
    char buffer[MAX_MESSAGE_LENGTH];
    memset(buffer,'\0',MAX_MESSAGE_LENGTH*sizeof(char));
    fileRead(filepath,buffer);

    char *token;
    char key_str[MAX_MESSAGE_LENGTH/2]; 
    char n_str[MAX_MESSAGE_LENGTH/2]; 
                                      
    token = strtok(buffer, ",");
    
    if (token != NULL) {
        strcpy(key_str, token); 
    }

    token = strtok(NULL, ",");
    if (token != NULL) {
        strcpy(n_str, token); 
    }

    unsigned long key = atoi(key_str);
    unsigned long n = atoi(n_str);

    printf("from %s found key: %li and modulus %li\n",filepath,key,n);


    /* convert text to array */
    int length = strlen(text);
    unsigned long int textArr[strlen(text)+1];
    memset(textArr,0,strlen(text+1)*sizeof(unsigned long int));
    for (int i =0; i<length; i++) {
        textArr[i] = (unsigned long int)text[i];
    }
    

    /*  transform message */
    unsigned long tmp[MAX_MESSAGE_LENGTH];
    transformArray(length,textArr, tmp, key,n);

    cipherarrayToString(tmp, length,returnBuffer);
}

void decrypt(char cipherText[MAX_MESSAGE_LENGTH], char* filepath, char returnString[MAX_MESSAGE_LENGTH]) {
    /* read key and n */
    int length = countCommas(cipherText)+1;
    
    char buffer[MAX_MESSAGE_LENGTH];
    memset(buffer,'\0',MAX_MESSAGE_LENGTH*sizeof(char));
    fileRead(filepath,buffer);

    char *token;
    char key_str[MAX_MESSAGE_LENGTH/2]; 
    char n_str[MAX_MESSAGE_LENGTH/2]; 
                                      
    token = strtok(buffer, ",");
    
    if (token != NULL) {
        strcpy(key_str, token); 
    }

    token = strtok(NULL, ",");
    if (token != NULL) {
        strcpy(n_str, token); 
    }

    unsigned long key = atoi(key_str);
    unsigned long n = atoi(n_str);

    printf("from %s found key: %li and modulo %li\n",filepath,key,n);

    /* turn ciphertext into array */
    unsigned long int cipherArr[MAX_MESSAGE_LENGTH]; 
    memset(cipherArr,0,sizeof(cipherArr));


    int lengthptr = length;
    cipherstringToArray(cipherText, cipherArr, &lengthptr);


    /*  transform message */

    unsigned long int plainTextArr[MAX_MESSAGE_LENGTH];
    char plainText[MAX_MESSAGE_LENGTH];
    memset(plainText,'\0',sizeof(plainText));
    memset(plainTextArr,0,MAX_MESSAGE_LENGTH*sizeof(unsigned long int));

    transformArray(length,cipherArr, plainTextArr, key,n);

    arrayToString(length, plainTextArr, plainText);

    sprintf(returnString,"%s",plainText);
}

