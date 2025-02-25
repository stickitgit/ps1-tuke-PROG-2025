#include "playfair.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVXYZ"
#define MATRIX_SIZE 5

static void create_matrix(const char* key, char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    char alphabet[26] = ALPHA;
    int used[26] = {0};
    int row = 0, col = 0;
    
    for (int i = 0; key[i] != '\0'; i++) {
        if (key[i] == ' ')
            continue;
            
        char c = toupper(key[i]);
        if (c == 'W')
            c = 'V';
            
        int idx = c - 'A';
        if (idx >= 0 && idx < 26 && !used[idx]) {
            used[idx] = 1;
            matrix[row][col] = c;
            col++;
            if (col == MATRIX_SIZE) {
                col = 0;
                row++;
            }
        }
    }
    
    for (int i = 0; alphabet[i] != '\0'; i++) {
        char c = alphabet[i];
        int idx = c - 'A';
        if (idx >= 0 && idx < 26 && !used[idx]) {
            used[idx] = 1;
            matrix[row][col] = c;
            col++;
            if (col == MATRIX_SIZE) {
                col = 0;
                row++;
            }
        }
    }
}

static void find_position(char matrix[MATRIX_SIZE][MATRIX_SIZE], char c, int* row, int* col) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (matrix[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

static char* prepare_text(const char* text) {
    int len = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ')
            len++;
    }
    
    char* prepared = (char*)malloc(2 * len + 1); 
    if (!prepared)
        return NULL;
    
    int j = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ')
            continue;
            
        char c = toupper(text[i]);
        if (c == 'W')
            c = 'V';
            
        prepared[j++] = c;
    }
    prepared[j] = '\0';
    
    int original_len = j;
    j = 0;
    for (int i = 0; i < original_len; i++) {
        char c = prepared[i];
        prepared[j++] = c;
        
        if (i + 1 < original_len && prepared[i + 1] == c && c != 'X') {
            prepared[j++] = 'X';
        }
    }
    prepared[j] = '\0';
    
    if (j % 2 != 0) {
        prepared[j++] = 'X';
        prepared[j] = '\0';
    }
    
    return prepared;
}

char* playfair_encrypt(const char* key, const char* text) {
    if (!key || !text)
        return NULL;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (!isalpha(text[i]) && text[i] != ' ')
            return NULL;
    }
    
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    create_matrix(key, matrix);
    
    char* prepared = prepare_text(text);
    if (!prepared)
        return NULL;
    
    int len = strlen(prepared);
    char* encrypted = (char*)malloc(2 * len + 1);
    if (!encrypted) {
        free(prepared);
        return NULL;
    }
    
    int enc_idx = 0;
    for (int i = 0; i < len; i += 2) {
        char a = prepared[i];
        char b = prepared[i + 1];
        
        int row_a, col_a, row_b, col_b;
        find_position(matrix, a, &row_a, &col_a);
        find_position(matrix, b, &row_b, &col_b);
        
        if (row_a == row_b) {
            encrypted[enc_idx++] = matrix[row_a][(col_a + 1) % MATRIX_SIZE];
            encrypted[enc_idx++] = ' ';
            encrypted[enc_idx++] = matrix[row_b][(col_b + 1) % MATRIX_SIZE];
        } else if (col_a == col_b) {
            encrypted[enc_idx++] = matrix[(row_a + 1) % MATRIX_SIZE][col_a];
            encrypted[enc_idx++] = ' ';
            encrypted[enc_idx++] = matrix[(row_b + 1) % MATRIX_SIZE][col_b];
        } else {
            encrypted[enc_idx++] = matrix[row_a][col_b];
            encrypted[enc_idx++] = ' ';
            encrypted[enc_idx++] = matrix[row_b][col_a];
        }
        
        if (i + 2 < len) {
            encrypted[enc_idx++] = ' ';
        }
    }
    
    encrypted[enc_idx] = '\0';
    free(prepared);
    return encrypted;
}

char* playfair_decrypt(const char* key, const char* text) {
    if (!key || !text)
        return NULL;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (toupper(text[i]) == 'W')
            return NULL;
    }
    
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    create_matrix(key, matrix);
    
    int char_count = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ')
            char_count++;
    }
    
    char* cipher_chars = (char*)malloc(char_count + 1);
    if (!cipher_chars)
        return NULL;
    
    int j = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ') {
            cipher_chars[j++] = toupper(text[i]);
        }
    }
    cipher_chars[j] = '\0';
    
    char* decrypted = (char*)malloc(char_count + 1);
    if (!decrypted) {
        free(cipher_chars);
        return NULL;
    }
    
    int dec_idx = 0;
    for (int i = 0; i < char_count; i += 2) {
        char a = cipher_chars[i];
        char b = (i + 1 < char_count) ? cipher_chars[i + 1] : 'X';
        
        int row_a, col_a, row_b, col_b;
        find_position(matrix, a, &row_a, &col_a);
        find_position(matrix, b, &row_b, &col_b);
        
        if (row_a == row_b) {
            decrypted[dec_idx++] = matrix[row_a][(col_a + MATRIX_SIZE - 1) % MATRIX_SIZE];
            decrypted[dec_idx++] = matrix[row_b][(col_b + MATRIX_SIZE - 1) % MATRIX_SIZE];
        } else if (col_a == col_b) {
            decrypted[dec_idx++] = matrix[(row_a + MATRIX_SIZE - 1) % MATRIX_SIZE][col_a];
            decrypted[dec_idx++] = matrix[(row_b + MATRIX_SIZE - 1) % MATRIX_SIZE][col_b];
        } else {
            decrypted[dec_idx++] = matrix[row_a][col_b];
            decrypted[dec_idx++] = matrix[row_b][col_a];
        }
    }
    
    decrypted[dec_idx] = '\0';
    free(cipher_chars);
    return decrypted;
}