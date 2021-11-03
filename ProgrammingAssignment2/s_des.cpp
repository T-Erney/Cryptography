#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <bitset>

template <int I>
void printb(uint64_t n) {
  std::bitset<I> x(n);
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

  return e;
}

uint8_t F(uint8_t X, uint8_t K) {
  uint8_t e = E(X);
  uint8_t e_xor_k = e ^ K;

  uint8_t sbox_1 = sbox1[e_xor_k >> 7][0x07 & (e_xor_k >> 4)];
  uint8_t sbox_2 = sbox2[(0x0f & e_xor_k) >> 3][0x07 & e_xor_k];

  return (sbox_1 << 3) + sbox_2;
}

uint16_t sdes_encrypt(uint16_t P, uint16_t K, uint8_t max_rounds) {
  uint8_t round = 0;

  uint8_t L = (uint8_t)(P >> 6);
  uint8_t R = (uint8_t)(0x3f & P);

  for (round = 0; round < max_rounds; round += 1) {
    uint8_t K_ = ((K << round) | (K >> (9 - round))) >> 1;

    std::cout << "\tKey for encryption :: ";
    printb<8>(K_);
    std::cout << "\n";

    uint8_t L_ = R;
    uint8_t R_ = L ^ F(R, K_);

    L = L_;
    R = R_;
  }

  return (R << 6) + L;
}

uint16_t sdes_decrypt(uint16_t C, uint16_t K, int8_t max_rounds) {
  int8_t round = max_rounds - 1;

  uint8_t R = (uint8_t)(C >> 6);
  uint8_t L = (uint8_t)(0x3f & C);

  for (; round >= 0; round -= 1) {
    uint8_t K_ = ((K << round) | (K >> (9 - round))) >> 1;

    std::cout << "\tKey for decryption :: ";
    printb<8>(K_);
    std::cout << "\n";

    uint8_t R_ = L;
    uint8_t L_ = R ^ F(R_, K_);

    L = L_;
    R = R_;

  }

  return (L << 6) + R;
}



int main() {

  std::cout << "1-4 rounds of Simple DES Encryption & Decryption\n";
  std::cout << "************************************************\n\n";

  uint16_t P = 0x08b5;
  uint16_t K = 0x01c7;

  std::cout << "Plaintext :: ";
  printb<12>(P);
  std::cout << "\n";

  std::cout << "Key       :: ";
  printb<12>(K);
  std::cout << "\n\n";

  for (int i = 1; i <= 4; i += 1) {
    std::cout << i << " rounds of Encryption & Decryption\n";

    uint16_t C = sdes_encrypt(P, K, i);
    std::cout << "  C after " << i << " rounds of encryption :: ";
    printb<12>(C);
    std::cout << "\n\n";

    uint16_t P_ = sdes_decrypt(C, K, i);
    std::cout << "  P after " << i << " rounds of decryption :: ";
    printb<12>(P_);
    std::cout << "\n\n" << "------------------------------\n\n";
  }

  return 0;
}
