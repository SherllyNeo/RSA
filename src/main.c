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

    /*
        encrypt message 
     */
    long int cipherText = transform(message,public_key_e,n);

    /*
        decrypt message 
     */
    long int decipherText = transform(cipherText,private_key_d,n);


    printf("Message to encrypt %li, CipherText: %li, DecipheredText %li\n",message,cipherText,decipherText);
    assert(decipherText == message);


    return 0;
}
