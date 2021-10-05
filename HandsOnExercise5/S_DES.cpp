#include <iostream>
#include <iomanip>
#include <bitset>
#include <stdlib.h>
#include <stdint.h>


void printb(uint8_t b) {
  std::bitset<8> x(b);
  std::cout << x;
}

uint8_t sbox1[2][8] = {
                       { 5, 2, 1, 6, 3, 4, 7, 0 },
                       { 1, 4, 6, 2, 0, 7, 5, 3 }
};

uint8_t sbox2[2][8] = {
                       { 4, 0, 6, 5, 7, 1, 3, 2 },
                       { 5, 3, 0, 7, 6, 2, 1, 4 }
};

uint8_t E(uint8_t X) {
  uint8_t e = 0;

  e += 0x20 & X;
  e += 0x10 & X;
  e += 0x04 & X;
  e += 0x08 & X;

  e += 0x04 & X;
  e += 0x08 & X;
  e += 0x02 & X;
  e += 0x01 & X;

  return e;
}

uint8_t F(uint8_t X, uint8_t K) {
  uint8_t e = E(X);
  uint8_t e_xor_k = e ^ K;

  uint8_t sbox_1 = sbox1[((0xf0 & e_xor_k) >> 4) >> 3][(0x07 & (0xf0 & e_xor_k)) >> 4];
  uint8_t sbox_2 = sbox2[(0x0f & e_xor_k) >> 3][(0x07 & (0x0f & e_xor_k))];

  return (sbox_1 << 3) + sbox_2;
}

char* sdes_encrypt(uint16_t P, uint16_t K) {
  uint8_t round = 0;
  uint8_t max_rounds = 2;

  // P = 0bxxxx |oooo oo|oo oooo|
  uint8_t L = (uint8_t)((0x0fc0 & P) >> 6);
  uint8_t R = (uint8_t)(0x0006 & P);

  for (round = 0; round < max_rounds; round += 1) {
    
  }
}

char* sdes_decrypt(uint16_t C, uint16_t K) { }

int main() {

  {
    uint16_t K = 0x0099;  // 0 1001 1001
    uint16_t P = 0x0726; // 011100 100110
    uint16_t C = 0;

    std::cout << "Plaintext: ";
    printb(P >> 8);
    printb((uint8_t)P);
    std::cout << "\n";
    std::cout << "Key:       ";
    printb((uint8_t)(K >> 8));
    printb((uint8_t)(K));
    std::cout << "\n";


  }

  return 0;
}
