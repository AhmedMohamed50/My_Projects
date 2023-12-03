/*#include <stdio.h>
#include <stdint.h>

unsigned char sbox[] = {
    0x9, 0x4, 0xA, 0xB,
    0xD, 0x1, 0x8, 0x5,
    0x6, 0x2, 0x0, 0x3,
    0xC, 0xE, 0xF, 0x7
};

void rotateNibbles(unsigned char *word);
void subNibbles(unsigned char *word);
unsigned char g(unsigned char w, unsigned char RCON_value);
void keyGeneration(unsigned short key, unsigned char *roundKeys);

void Expand_key(int key0, int *key1, int *key2);

int main() {
    unsigned short plaintext = 0xD728;
    unsigned short key = 0x4AF5;

    int key1, key2;
    Expand_key(key, &key1, &key2);

    printf("\nExpanded Keys:\n");
    printf("Key0: %04X\n", key);
    printf("Key1: %04X\n", key1);
    printf("Key2: %04X\n", key2);

    return 0;
}

void rotateNibbles(unsigned char *word) {
    unsigned char temp = (word[0] & 0x0F) << 4;
    word[0] = (word[0] & 0xF0) >> 4;
    word[0] |= temp;
}

void subNibbles(unsigned char *word) {
    word[0] = sbox[word[0] >> 4] << 4 | sbox[word[0] & 0x0F];
}

unsigned char g(unsigned char w, unsigned char RCON_value) {
    unsigned char N[2];
    rotateNibbles(&w);
    N[0] = sbox[(w >> 4) & 0x0F];
    N[1] = sbox[w & 0x0F];
    unsigned char compined_N = (N[0] << 4) | N[1];
    unsigned char w_dash = compined_N ^ RCON_value;
    return w_dash;
}

void keyGeneration(unsigned short key, unsigned char *roundKeys) {
    unsigned char w0, w1, w2, w3, w4, w5;

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

void Expand_key(int key0, int *key1, int *key2) {
    unsigned char w[2];
    w[1] = key0 & 0xff;
    w[0] = (key0 >> 8) & 0xff;
    unsigned char w1_g = g(w[1], 0x80);
    unsigned char w2 = w[0] ^ w1_g;
    unsigned char w3 = w[1] ^ w2;
    *key1 = (w2 << 8) | w3;
    unsigned char w3_g = g(w3, 0x30);
    unsigned char w4 = w2 ^ w3_g;
    unsigned char w5 = w3 ^ w4;
    *key2 = (w4 << 8) | w5;
}*/
