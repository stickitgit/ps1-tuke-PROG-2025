#include "bmp.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* reverse(const char* text) {
    if (!text) {
        return NULL;
    }
    
    int len = strlen(text);
    char* result = (char*)malloc(len + 1);
    if (!result) {
        return NULL;
    }
    
    for (int i = 0; i < len; i++) {
        result[i] = toupper(text[len - 1 - i]);
    }
    result[len] = '\0';
    
    return result;
}

char* vigenere_encrypt(const char* key, const char* text) {
    if (!key || !text) {
        return NULL;
    }
    
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {
            return NULL;
        }
    }
    
    int text_len = strlen(text);
    int key_len = 0;
    
    for (int i = 0; key[i] != '\0'; i++) {
        if (isalpha(key[i])) {
            key_len++;
        }
    }
    
    if (key_len == 0) {
        return NULL;
    }
    
    char* result = (char*)malloc(text_len + 1);
    if (!result) {
        return NULL;
    }
    
    int key_index = 0;
    for (int i = 0; i < text_len; i++) {
        char c = text[i];
        
        if (isalpha(c)) {
            char key_char;
            int j = 0;
            while (1) {
                key_char = key[(key_index + j) % strlen(key)];
                if (isalpha(key_char)) {
                    key_index = (key_index + j + 1) % strlen(key);
                    break;
                }
                j++;
            }
            
            c = toupper(c);
            key_char = toupper(key_char);
            
            int shift = key_char - 'A';
            int original_pos = c - 'A';
            
            int new_pos = (original_pos + shift) % 26;
            result[i] = 'A' + new_pos;
        } else {
            result[i] = c;
        }
    }
    
    result[text_len] = '\0';
    return result;
}

char* vigenere_decrypt(const char* key, const char* text) {
    if (!key || !text) {
        return NULL;
    }
    
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {
            return NULL;
        }
    }
    
    int text_len = strlen(text);
    int key_len = 0;
    
    for (int i = 0; key[i] != '\0'; i++) {
        if (isalpha(key[i])) {
            key_len++;
        }
    }
    
    if (key_len == 0) {
        return NULL;
    }
    
    char* result = (char*)malloc(text_len + 1);
    if (!result) {
        return NULL;
    }
    
    int key_index = 0;
    for (int i = 0; i < text_len; i++) {
        char c = text[i];
        
        if (isalpha(c)) {
            char key_char;
            int j = 0;
            while (1) {
                key_char = key[(key_index + j) % strlen(key)];
                if (isalpha(key_char)) {
                    key_index = (key_index + j + 1) % strlen(key);
                    break;
                }
                j++;
            }
            
            c = toupper(c);
            key_char = toupper(key_char);
            
            int shift = key_char - 'A';
            int original_pos = c - 'A';
            
            int new_pos = (original_pos - shift + 26) % 26;
            result[i] = 'A' + new_pos;
        } else {
            result[i] = c;
        }
    }
    
    result[text_len] = '\0';
    return result;
}

unsigned char* bit_encrypt(const char* text) {
    if (!text) {
        return NULL;
    }
    
    int len = strlen(text);
    unsigned char* result = (unsigned char*)malloc(len + 1);
    if (!result) {
        return NULL;
    }
    
    for (int i = 0; i < len; i++) {
        unsigned char c = text[i];
        unsigned char high_nibble = (c >> 4) & 0x0F;
        unsigned char low_nibble = c & 0x0F;
        
        unsigned char swapped_high = 0;
        swapped_high |= ((high_nibble & 0x01) << 1) | ((high_nibble & 0x02) >> 1);
        swapped_high |= ((high_nibble & 0x04) << 1) | ((high_nibble & 0x08) >> 1);
        
        unsigned char new_low = low_nibble ^ swapped_high;
        
        result[i] = (swapped_high << 4) | new_low;
    }
    
    result[len] = '\0';
    return result;
}

char* bit_decrypt(const unsigned char* text) {
    if (!text) {
        return NULL;
    }
    
    int len = 0;
    while (text[len] != '\0') {
        len++;
    }
    
    char* result = (char*)malloc(len + 1);
    if (!result) {
        return NULL;
    }
    
    for (int i = 0; i < len; i++) {
        unsigned char c = text[i];
        unsigned char swapped_high = (c >> 4) & 0x0F;
        unsigned char xor_low = c & 0x0F;
        
        unsigned char low_nibble = xor_low ^ swapped_high;
        
        unsigned char high_nibble = 0;
        high_nibble |= ((swapped_high & 0x01) << 1) | ((swapped_high & 0x02) >> 1);
        high_nibble |= ((swapped_high & 0x04) << 1) | ((swapped_high & 0x08) >> 1);
        
        result[i] = (high_nibble << 4) | low_nibble;
    }
    
    result[len] = '\0';
    return result;
}

unsigned char* bmp_encrypt(const char* key, const char* text) {
    if (!key || !text) {
        return NULL;
    }
    
    char* reversed = reverse(text);
    if (!reversed) {
        return NULL;
    }
    
    char* vigenere = vigenere_encrypt(key, reversed);
    free(reversed);
    if (!vigenere) {
        return NULL;
    }
    
    unsigned char* result = bit_encrypt(vigenere);
    free(vigenere);
    
    return result;
}

char* bmp_decrypt(const char* key, const unsigned char* text) {
    if (!key || !text) {
        return NULL;
    }
    
    char* bit_decrypted = bit_decrypt(text);
    if (!bit_decrypted) {
        return NULL;
    }
    
    char* vigenere_decrypted = vigenere_decrypt(key, bit_decrypted);
    free(bit_decrypted);
    if (!vigenere_decrypted) {
        return NULL;
    }
    
    char* result = reverse(vigenere_decrypted);
    free(vigenere_decrypted);
    
    return result;
}