#include <stdio.h>
#include <stdlib.h>
#include "playfair.h"
#include "bmp.h"

void test_playfair() {
    printf("\n=== Testing Playfair Cipher ===\n");
    
    char *encrypted, *decrypted;

    encrypted = playfair_encrypt("SeCReT", "Hello world");
    printf("Encrypted: %s\n", encrypted);
    decrypted = playfair_decrypt("SeCReT", encrypted);
    printf("Decrypted: %s\n", decrypted);
    free(encrypted);
    free(decrypted);

    encrypted = playfair_encrypt("world", "Hello");
    printf("Encrypted: %s\n", encrypted);
    decrypted = playfair_decrypt("world", encrypted);
    printf("Decrypted: %s\n", decrypted);
    free(encrypted);
    free(decrypted);

    encrypted = playfair_encrypt("Password", "Taxi please");
    printf("Encrypted: %s\n", encrypted);
    decrypted = playfair_decrypt("Password", encrypted);
    printf("Decrypted: %s\n", decrypted);
    free(encrypted);
    free(decrypted);

    encrypted = playfair_encrypt("please", "Taxxxiii");
    printf("Encrypted: %s\n", encrypted);
    decrypted = playfair_decrypt("please", encrypted);
    printf("Decrypted: %s\n", decrypted);
    free(encrypted);
    free(decrypted);
}

void test_reverse() {
    printf("\n=== Testing Reverse Function ===\n");
    
    char* reversed = reverse("Hello world!");
    printf("Reversed: %s\n", reversed);
    free(reversed);
}

void test_vigenere() {
    printf("\n=== Testing Vigenère Cipher ===\n");
    
    char* encrypted = vigenere_encrypt("CoMPuTeR", "Hello world!");
    printf("Encrypted: %s\n", encrypted);
    
    char* decrypted = vigenere_decrypt("CoMPuTeR", encrypted);
    printf("Decrypted: %s\n", decrypted);
    
    free(encrypted);
    free(decrypted);
}

void test_bit_chaos() {
    printf("\n=== Testing Bit Chaos ===\n");
    
    unsigned char* encrypted = bit_encrypt("Hello world!");
    printf("Encrypted (hex): ");
    for(int i = 0; i < 12; i++) {
        printf("%02x ", encrypted[i]);
    }
    printf("\n");
    
    char* decrypted = bit_decrypt(encrypted);
    printf("Decrypted: %s\n", decrypted);
    
    free(encrypted);
    free(decrypted);
}

void test_bmp() {
    printf("\n=== Testing BMP Cipher ===\n");
    
    unsigned char* encrypted = bmp_encrypt("TopSecret", "This is a secret message!");
    printf("Encrypted (hex): ");
    int len = 0;
    while(encrypted[len] != '\0') len++;
    for(int i = 0; i < len; i++) {
        printf("%02x ", encrypted[i]);
    }
    printf("\n");
    
    char* decrypted = bmp_decrypt("TopSecret", encrypted);
    printf("Decrypted: %s\n", decrypted);
    
    free(encrypted);
    free(decrypted);
}

int main() {
    printf("=== Cryptography Project ===\n");
    
    test_playfair();
    test_reverse();
    test_vigenere();
    test_bit_chaos();
    test_bmp();
    
    return 0;
}