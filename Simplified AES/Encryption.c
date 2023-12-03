#include <stdio.h>
#include <string.h>
#include <stdbool.h>

unsigned short sbox[] = {
    0x9, 0x4, 0xA, 0xB,
    0xD, 0x1, 0x8, 0x5,
    0x6, 0x2, 0x0, 0x3,
    0xC, 0xE, 0xF, 0x7
};

unsigned short invsbox[] = {
    0xA, 0x5, 0x9, 0xB,
    0x1, 0x7, 0x8, 0xF,
    0x6, 0x0, 0x2, 0x3,
    0xC, 0x4, 0xD, 0xE
};

void rotateNibbles(unsigned short *word);
void subNibbles(unsigned short *word);
unsigned short g(unsigned short w, unsigned short RCON_value);
void keyGeneration(unsigned short key, unsigned short *roundKeys);
unsigned short addRoundKey(unsigned short state, unsigned short roundKey);
void subBytes(unsigned short *state);
void shiftRows(unsigned short *state);
void mixColumns(unsigned short *state);
unsigned short Compine(unsigned short *in);
unsigned short gmul(unsigned short a, unsigned short b);
void Expand_key(unsigned short key0, unsigned short *key1, unsigned short *key2);
unsigned short encryption (unsigned short key, unsigned short plaintext);
void invSubBytes(unsigned short *state);
void invMix_columns(unsigned short *s, unsigned short *mixed_s);
unsigned short decryption (unsigned short key, unsigned short ciphertext);
bool check_hexa(char *key, char *text);

int main(int argc, char* argv[])
{
    // check number of command arguments
    if(argc != 4) {
        printf("wrong number of command arguments");
        return -1;
    }

    // check if non-hex characters are found
    else if(!check_hexa(argv[2], argv[3])) {
        printf("non-hex characters are found");
        return -1;
    }

    // Encryption
    else if(strcmp(argv[1], "ENC") == 0) {
        unsigned short key = (unsigned short)strtol(argv[2], NULL, 16);
        unsigned short plaintext = (unsigned short)strtol(argv[3], NULL, 16);
        unsigned short ciphertext = encryption(key, plaintext);
        printf("%.4X", ciphertext);
    }

    // Decryption
    else if(strcmp(argv[1], "DEC") == 0) {
        unsigned short key = (unsigned short)strtol(argv[2], NULL, 16);
        unsigned short ciphertext_D = (unsigned short)strtol(argv[3], NULL, 16);
        unsigned short plaintext_D = decryption(key, ciphertext_D);
        printf("%.4X", plaintext_D);
    }

	return 0;
}
unsigned short encryption (unsigned short key, unsigned short plaintext){

    unsigned short out[4];
    unsigned short key1, key2, addout;
    Expand_key(key, &key1, &key2);

    // AddRoundKey for Key0
    addout = addRoundKey(plaintext, key);

    // SubBytes
    Split(addout,out);
    subBytes(out);

    // ShiftRows
    shiftRows(out);

    // MixColumns
    unsigned short mixed[4];
    Mix_columns(out,mixed);

    // AddRoundKey for Key1
    unsigned short mix;
    mix = Compine(mixed);
    addout = addRoundKey(mix, key1);

    // nibble substitution
    Split(addout,out);
    subBytes(out);

    // ShiftRows
    shiftRows(out);

    // AddRoundKey for Key2
    mix = Compine(out);
    addout = addRoundKey(mix, key2);
}

unsigned short decryption (unsigned short key, unsigned short ciphertext){

    unsigned short out[4];
    unsigned short key1, key2, addout,mix;
    Expand_key(key, &key1, &key2);

    // AddRoundKey for Key2
    addout = addRoundKey(ciphertext, key2);

    // ShiftRows
    Split(addout,out);
    shiftRows(out);

    // inv nibble substitution
    invSubBytes(out);

    // AddRoundKey for Key1
    mix = Compine(out);
    addout = addRoundKey(mix, key1);

    // MixColumns
    unsigned short mixed[4];
    Split(addout,out);
    invMix_columns(out,mixed);

    // ShiftRows
    shiftRows(mixed);

    // inv nibble substitution
    invSubBytes(mixed);


    // AddRoundKey for Key0
    mix = Compine(mixed);
    addout = addRoundKey(mix, key);
}

void Mix_columns(unsigned short *s, unsigned short *mixed_s) {

    mixed_s[0] = s[0] ^ gmul(0x4, s[1]);
    mixed_s[1] = gmul(0x4, s[0]) ^ s[1];
    mixed_s[2] = s[2] ^ gmul(0x4, s[3]);
    mixed_s[3] = gmul(0x4, s[2]) ^ s[3];
}

void invMix_columns(unsigned short *s, unsigned short *mixed_s) {

    mixed_s[0] = gmul(0x9, s[0]) ^ gmul(0x2, s[1]);
    mixed_s[1] = gmul(0x2, s[0]) ^ gmul(0x9, s[1]);
    mixed_s[2] = gmul(0x9, s[2]) ^ gmul(0x2, s[3]);
    mixed_s[3] = gmul(0x2, s[2]) ^ gmul(0x9, s[3]);
}

unsigned short Compine(unsigned short *in) {
    return (in[0]<<12) | (in[1]<<8) | (in[2]<<4) | in[3];
}

unsigned short gmul(unsigned short a, unsigned short b) {
    unsigned short p = 0; /* accumulator for the product of the multiplication */
    while (a != 0 && b != 0) {
        if (b & 1) /* if the polynomial for b has a constant term, add the corresponding a to p */
            p ^= a; /* addition in GF(2^m) is an XOR of the polynomial coefficients */

        if (a & 0b1000) /* GF modulo: if a has a nonzero term x^3, then must be reduced when it becomes x^4 */
            a = (a << 1) ^ 0b10011; /* subtract (XOR) the primitive polynomial x^4 + x + 1 (0b10011) */
        else
            a <<= 1; /* equivalent to a*x */
        b >>= 1;
    }
    return p;
}

void Split(unsigned short state, unsigned short *out) {
    out[0] = (state>>12) & 0xf;
    out[1] = (state>>8) & 0xf;
    out[2] = (state>>4) & 0xf;
    out[3] = state & 0xf;
}

void shiftRows(unsigned short *state) {
    unsigned short temp = state[1];
    state[1] = state[3];
    state[3] = temp;
}

void subBytes(unsigned short *state) {
    for (int i=0;i<4;i++){
        state[i] = sbox[state[i]];}
}

void invSubBytes(unsigned short *state) {
    for (int i=0;i<4;i++){
        state[i] = invsbox[state[i]];}
}

void rotateNibbles(unsigned short *word) {
    unsigned short temp = (word[0] & 0x0F) << 4;
    word[0] = (word[0] & 0xF0) >> 4;
    word[0] |= temp;
}

void subNibbles(unsigned short *word) {
    word[0] = sbox[word[0] >> 4] << 4 | sbox[word[0] & 0x0F];
}

unsigned short g(unsigned short w, unsigned short RCON_value) {
    unsigned short N[2];
    rotateNibbles(&w);
    N[0] = sbox[(w >> 4) & 0x0F];
    N[1] = sbox[w & 0x0F];
    unsigned short compined_N = (N[0] << 4) | N[1];
    unsigned short w_dash = compined_N ^ RCON_value;
    return w_dash;
}

void keyGeneration(unsigned short key, unsigned short *roundKeys) {
    unsigned short w0, w1, w2, w3, w4, w5;

    w0 = (key >> 8) & 0xFF;
    w1 = key & 0xFF;

    roundKeys[0] = w0;
    roundKeys[1] = w1;

    rotateNibbles(&w1);
    subNibbles(&w1);

    w2 = w0 ^ 0x80 ^ w1;
    w3 = w2 ^ w1;
    roundKeys[2] = w2;
    roundKeys[3] = w3;

    rotateNibbles(&w3);
    subNibbles(&w3);

    w4 = w2 ^ 0x30 ^ w3;
    w5 = w4 ^ w3;
    roundKeys[4] = w4;
    roundKeys[5] = w5;
}

unsigned short addRoundKey(unsigned short state, unsigned short roundKey) {
    return  state ^ roundKey;
}

void Expand_key(unsigned short key0, unsigned short *key1, unsigned short *key2) {
    unsigned short w[2];
    w[1] = key0 & 0xff;
    w[0] = (key0 >> 8) & 0xff;
    unsigned short w1_g = g(w[1], 0x80);
    unsigned short w2 = w[0] ^ w1_g;
    unsigned short w3 = w[1] ^ w2;
    *key1 = (w2 << 8) | w3;
    unsigned short w3_g = g(w3, 0x30);
    unsigned short w4 = w2 ^ w3_g;
    unsigned short w5 = w3 ^ w4;
    *key2 = (w4 << 8) | w5;
}

bool check_hexa(char *key, char *text) {
    char keyString[4];
    strcpy(keyString, key);
    char *keyEndptr;
    long keyValue = strtol(keyString, &keyEndptr, 16);

    if (*keyEndptr != '\0') {
        return 0;
    }

    char textString[4];
    strcpy(textString, text);
    char *textEndptr;
    long textValue = strtol(textString, &textEndptr, 16);

    if (*textEndptr != '\0') {
        return 0;
    }

    return 1;
}
