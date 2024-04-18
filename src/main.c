#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

long int gcd(long int a, long int b) {
    while (b != 0) {
        long int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool areCoprime(long int a, long int b) {
    return gcd(a, b) == 1;
}

#define MAX_MESSAGE_LENGTH 1024

bool isPrime(long int num) {
    if (num <= 1)
        return false;
    for (long int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

long int generateRandomPrime(long int lower, long int upper) {
    srand(time(NULL));

    long int num = (rand() % (upper - lower + 1)) + lower;

    if (num % 2 == 0)
        num++;

    while (!isPrime(num)) {
        num += 2; 
        if (num > upper) 
            num = lower + (num - upper - 1);
    }

    return num;
}

long int modExp(long int base, long int exponent, long int modulus) {
    long int result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent & 1)
            result = (result * base) % modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

long int modInverse(long int a, long int m) {
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




long int findPublicKey(long int phi_n) {
    long int public_key = 65537; 
    if (public_key < phi_n && areCoprime(public_key, phi_n))
        return public_key;
    else {
        for (long int i = 3; i < phi_n; i += 2) {
            if (i < phi_n && areCoprime(i, phi_n))
                return i;
        }
    }
    return -1; 
}

long int transforM(long int message,long int power, long int modulus) {
    return modExp(message, power, modulus);
}

long int transform(long int message, long int exponent, long int modulus) {
    return modExp(message, exponent, modulus);
}

void transformArray(int inputLen,long int input[MAX_MESSAGE_LENGTH], long int output[MAX_MESSAGE_LENGTH],long int exponent, long int modulus) {
    for (int i = 0; i < inputLen; i++) {
        long int trans = transform(input[i], exponent, modulus);
        output[i] = trans; 
    }
}

void printArray(int length, int long arr[MAX_MESSAGE_LENGTH]) {
    for (int i =0; i<length;i++) {
        printf("%c ",(char)arr[i]);
    }
    printf("\n");
}

void arrayToString(int length, int long arr[MAX_MESSAGE_LENGTH],char string[MAX_MESSAGE_LENGTH]) {
    for (int i =0; i<length;i++) {
        string[i] = (char)arr[i];
    }
}


int main() {
    long int p, q;
    p = generateRandomPrime(10000, 20000);
    q = generateRandomPrime(10000, 20000);
    while (p == q) {
        q = generateRandomPrime(10000, 20000);
    }
    long int n = p * q;
    long int phi_n = (p - 1) * (q - 1);
    long int public_key_e = findPublicKey(phi_n);
    long int private_key_d = modInverse(public_key_e, phi_n);

    printf("Using p: %li and q: %li, found n: %li\n using totient %li and public key %li we calculated private key %li\n\n",
           p, q, n, phi_n, public_key_e, private_key_d);

    long int message = rand() % 10000;
    char* plainText = "HEY";
    int length = strlen(plainText);
    long int plainTextArr[strlen(plainText)+1];
    memset(plainTextArr,0,strlen(plainText+1)*sizeof(long int));
    for (int i =0; i<length; i++) {
        plainTextArr[i] = (long int)plainText[i];
    }
    

    /*
        encrypt message 
     */
    long int cipherNumber = transform(message,public_key_e,n);
    long int cipherTextArr[strlen(plainText)+1];
    char cipherText[MAX_MESSAGE_LENGTH];
    memset(cipherText,'\0',sizeof(cipherText));
    memset(cipherTextArr,0,strlen(plainText+1)*sizeof(long int));
    transformArray(length,plainTextArr, cipherTextArr, public_key_e,n);
    arrayToString(length, cipherTextArr, cipherText);

    /*
        decrypt message 
     */
    long int decipherNumber = transform(cipherNumber,private_key_d,n);
    long int decipherTextArr[strlen(plainText)+1];
    char decipherText[MAX_MESSAGE_LENGTH];
    memset(decipherText,'\0',sizeof(decipherText));
    memset(plainTextArr,0,strlen(plainText+1)*sizeof(long int));
    transformArray(length,cipherTextArr, decipherTextArr, private_key_d,n);
    arrayToString(length, decipherTextArr, decipherText);



    printf("Message to encrypt %li, CipherNumber: %li, DecipheredNumber %li\n",message,cipherNumber,decipherNumber);
    printf("Message to encrypt %s, CipherText: %s, DecipheredText: %s\n",plainText,cipherText,decipherText);
    assert(decipherNumber == message);


    return 0;
}
