#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


// Function to check if a number is prime
bool isPrime(int num) {
    if (num <= 1)
        return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return false;
    }
    return true;
}

// Function to generate a random prime number within a given range
int generateRandomPrime(int lower, int upper) {
    // Seed the random number generator
    srand(time(NULL));

    // Generate a random number within the range
    int num = (rand() % (upper - lower + 1)) + lower;

    // If the number is even, make it odd
    if (num % 2 == 0)
        num++;

    // Keep generating random numbers until a prime is found
    while (!isPrime(num)) {
        num += 2; // Increment by 2 to skip even numbers
        if (num > upper) // If we exceed the upper limit, start from lower limit again
            num = lower + (num - upper - 1);
    }

    return num;
}



long int modInverse( long int X, long int Y)
{
    for (int i = 1; i < Y; i++)
        if (((X % Y) * (i % Y)) % Y == 1) {
            return i;
        }
    return -1;
}


void encrypt_text(char* text, char* output,int public_key_e, int public_key_n) {
    for (int i = 0; i<(long int)strlen(text);i++) {
        if (text[i] == ' ') {
            output[i] = ' ';
        }
        else {
            long int encoded = toupper(text[i])-'A';
            output[i] = ((long int)pow(encoded,public_key_e) % public_key_n) + 'A';
        }
    }
}

void decrypt_text(char* text, char* output,int private_key_d, int public_key_n) {
    for (int i = 0; i<(long int)strlen(text);i++) {
        if (text[i] == ' ') {
            output[i] = ' ';
        }
        else {
            long int cipher_li = toupper(text[i])-'A';
            output[i] = ((long int)pow(cipher_li,private_key_d) % public_key_n)+'A';
        }
    }
}

// Function to calculate the greatest common divisor (GCD)
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to check if two numbers are coprime
bool areCoprime(int a, int b) {
    return gcd(a, b) == 1;
}

// Function to find the public key given the totient (phi) and n
int findPublicKey(int phi_n) {
    int public_key = 3; // Conventionally chosen public key
    if (areCoprime(public_key, phi_n))
        return public_key;
    else {
        // If not coprime, find the next coprime number
        for (int i = 3; i < phi_n; i += 2) {
            if (areCoprime(i, phi_n))
                return i;
        }
    }
    return -1; // Error: No coprime number found
}

 
int main(int argc, char *argv[])
{
    int messageAsInts[2024];
    /* generate two primes p and q, and make n their product which is a semi prime  */
    long int p = generateRandomPrime(0, 30);
    long int q = generateRandomPrime(0, 30);
    while (p == q) {
        q = generateRandomPrime(0, 30);
    }
    p = 17;
    q = 31;

    long int n = p*q;


    /* calculate the totient of n */
    long int phi_n = (p-1)*(q-1);

    /* 
     generate the public key, e, that follows 3 rules
     1. Must be below the totient and above 2
     2. must be coprime with the totient 
     3. 
     */

    long int e = findPublicKey(phi_n);

    /* 
    Generate the private key, d the modulo inverse of e with respect to totient of n
    */
    long int d = modInverse(e, phi_n);

    printf("Using p: %li and q: %li, found n: %li\n using totient %li and public key %li we calculated private key %li\n",p,q,n,phi_n,e,d);

    /*
     Encrypt paintext
     */
    char* plaintext = "The quick brown fox jumped over the lazy dog";
    char ciphertext[2024];
    memset(ciphertext,0,sizeof(ciphertext));
    encrypt_text(plaintext, ciphertext, e, n);
    printf("turned %s plaintext into %s ciphertext\n",plaintext,ciphertext);

    /*
     Decrypt cipher text
     */
    char decryptedplaintext[2024];
    memset(decryptedplaintext,0,sizeof(ciphertext));
    decrypt_text(ciphertext, decryptedplaintext, d, n);
    printf("turned %s ciphertext into %s plaintext\n",ciphertext,decryptedplaintext);


    /* working example with int 
    long int message = 8;
    long int ciphertexts = (long int)pow(message,e) % n;
    long int plaintexts = (long int)pow(ciphertexts,d) % n;
    printf("encrypting %li to get %li and then decryptng to get %li\n",message,ciphertexts,plaintexts);
    */

    return EXIT_SUCCESS;
}
