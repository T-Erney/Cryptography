#include <iostream>
#include <iomanip>
#include <bitset>
#include <stdlib.h>
#include <stdint.h>

void printb3(uint8_t b) {
  std::bitset<3> x(b);
  std::cout << x;
}
void printb6(uint8_t b) {
  std::bitset<6> x(b);
  std::cout << x;
}
void printb9(uint16_t b) {
  std::bitset<9> x(b);
  std::cout << x;
}
void printb12(uint16_t b) {
  std::bitset<12> x(b);
  std::cout << x;
}
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

  e |= ((0x20 & X) >> 5) << 7;
  e |= ((0x10 & X) >> 4) << 6;
  e |= ((0x04 & X) >> 2) << 5;
  e |= ((0x08 & X) >> 3) << 4;

  e |= ((0x04 & X) >> 2) << 3;
  e |= ((0x08 & X) >> 3) << 2;
  e |= ((0x02 & X) >> 1) << 1;
  e |= ((0x01 & X) >> 0) << 0;

  std::cout << "E(";
  printb6(X);
  std::cout << ") = ";
  printb(e);
  std::cout << "\n\n";

  return e;
}

uint8_t F(uint8_t X, uint8_t K) {
  uint8_t e = E(X);
  uint8_t e_xor_k = e ^ K;

  uint8_t sbox_1 = sbox1[e_xor_k >> 7][0x07 & (e_xor_k >> 4)];
  uint8_t sbox_2 = sbox2[(0x0f & e_xor_k) >> 3][0x07 & e_xor_k];

  std::cout << "sbox_1 = ";
  printb3(sbox_1);
  std::cout << "\n";
  std::cout << "sbox_2 = ";
  printb3(sbox_2);
  std::cout << "\n\n";

  return (sbox_1 << 3) + sbox_2;
}

uint16_t sdes_encrypt(uint16_t P, uint16_t K) {
  uint8_t round = 0;
  uint8_t max_rounds = 2;

  // P = 0bxxxx |oooo oo|oo oooo|
  uint8_t L = (uint8_t)(P >> 6);
  uint8_t R = (uint8_t)(0x3f & P);

  for (round = 0; round < max_rounds; round += 1) {
    std::cout << "Round " << round + 1 << "\n\n";
    std::cout << "L  = ";
    printb6(L);
    std::cout << "\n" << "R  = ";
    printb6(R);
    std::cout << "\n";

    uint8_t K_ = ((K << round) | (K >> 9 - round)) >> 1;

    std::cout << "K_ = ";
    printb(K_);
    std::cout << "\n\n";

    uint8_t L_ = R;
    uint8_t R_ = L ^ F(R, K_);

    L = L_;
    R = R_;
  }

  return (L << 6) + R;
}

uint16_t sdes_decrypt(uint16_t C, uint16_t K) {
  int8_t round = 1;
  uint8_t max_rounds = 0;

  uint8_t R = (uint8_t)(C >> 6);
  uint8_t L = (uint8_t)(0x3f & C);

  for (round = 1; round >= max_rounds; round -= 1) {
    std::cout << "Round " << round + 1 << "\n\n";
    std::cout << "L  = ";
    printb6(L);
    std::cout << "\n" << "R  = ";
    printb6(R);
    std::cout << "\n";

    uint8_t K_ = ((K << round) | (K >> 9 - round)) >> 1;

    std::cout << "K_ = ";
    printb(K_);
    std::cout << "\n\n";

    uint8_t R_ = L;
    uint8_t L_ = R ^ F(R_, K_);

    L = L_;
    R = R_;

  }

  return (L << 6) + R;
}

int main() {

  {
    std::cout << "#2\n\n";
    uint16_t K = 0x0099;  // 0 1001 1001
    uint16_t P = 0x0726; // 011100 100110

    std::cout << "Plaintext:  ";
    printb12(P);
    std::cout << "\n";
    std::cout << "Key:        ";
    printb9(K);
    std::cout << "\n";

    uint16_t C = sdes_encrypt(P, K);
    std::cout << "L2R2:       ";
    printb12(C);
    std::cout << "\n";
  }

  {
    std::cout << "#3\n\n";
    uint16_t K = 0x0099;
    uint16_t C = 0x0837;

    std::cout << "Ciphertext: ";
    printb12(C);
    std::cout << "\n";
    std::cout << "Key:        ";
    printb9(K);
    std::cout << "\n";

    uint16_t P = sdes_decrypt(C, K);
    std::cout << "L0R0:       ";
    printb12(P);
    std::cout << "\n";
  }

  return 0;
}
