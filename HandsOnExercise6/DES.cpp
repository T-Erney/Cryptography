#include <iostream>
#include <iomanip>
#include <bitset>
#include <stdlib.h>
#include <stdint.h>

template <uint64_t I>
std::string printb(uint64_t b) {
  std::bitset<I> x(b);
  return x.to_string();
}

// S-Boxes 1-8
uint8_t sboxes[][64] = {
                       {
                        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
                         0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
                         4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
                        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
                       },

                       {
                        15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
                         3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
                         0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
                        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
                       },

                       {
                        10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
                        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
                        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
                         1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
                       },

                       {
                         7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
                        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  7,
                        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
                         3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
                       },

                       {
                         2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
                        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
                         4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
                        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
                       },

                       {
                        12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
                        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
                         9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
                         4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
                       },

                       {
                         4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
                        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
                         1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
                         6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
                       },

                       {
                        13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
                         1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
                         7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
                         2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
                       }
};

// Permutations
uint8_t ip[] = {
                   58, 50, 42, 34, 26, 18, 10,  2,
                   60, 52, 44, 36, 28, 20, 12,  4,
                   62, 54, 46, 38, 30, 22, 14,  6,
                   64, 56, 48, 40, 32, 24, 16,  8,
                   57, 49, 41, 33, 25, 17,  9,  1,
                   59, 51, 43, 35, 27, 19, 11,  3,
                   61, 53, 45, 37, 29, 21, 13,  5,
                   63, 55, 47, 39, 31, 23, 15,  7
};

uint8_t fp[] = {
                  40,  8, 48, 16, 56, 24, 64, 32,
                  39,  7, 47, 15, 55, 23, 63, 31,
                  38,  6, 46, 14, 54, 22, 62, 30,
                  37,  5, 45, 13, 53, 21, 61, 29,
                  36,  4, 44, 12, 52, 20, 60, 28,
                  35,  3, 43, 11, 51, 19, 59, 27,
                  34,  2, 42, 10, 50, 18, 58, 26,
                  33,  1, 41,  9, 49, 17, 57, 25
};

uint8_t p[] =  {
                  16,  7, 20, 21, 29, 12, 28, 17,
                   1, 15, 23, 26,  5, 18, 31, 10,
                   2,  8, 24, 14, 32, 27,  3,  9,
                  19, 13, 30,  6, 22, 11,  4, 25
};

uint8_t pc1[] = {   // LEFT
                    57, 49, 41, 33, 25, 17,  9,
                     1, 58, 50, 42, 34, 26, 18,
                    10,  2, 59, 51, 43, 35, 27,
                    19, 11,  3, 60, 52, 44, 36,
                    // RIGHT
                    63, 55, 47, 39, 31, 23, 15,
                     7, 62, 54, 46, 38, 30, 22,
                    14,  6, 61, 53, 45, 37, 29,
                    21, 13,  5, 28, 20, 12,  4
};

uint8_t pc2[] = {
                  14, 17, 11, 24,  1,  5,
                   3, 28, 15,  6, 21, 10,
                  23, 19, 12,  4, 26,  8,
                  16,  7, 27, 20, 13,  2,
                  41, 52, 31, 37, 47, 55,
                  30, 40, 51, 45, 33, 48,
                  44, 49, 39, 56, 34, 53,
                  46, 42, 50, 36, 29, 32
};

uint64_t E(uint32_t x) {
  uint8_t ep[] = {
                  31,  0,  1,  2,  3,  4,
                   3,  4,  5,  6,  7,  8,
                   7,  8,  9, 10, 11, 12,
                  11, 12, 13, 14, 15, 16,
                  15, 16, 17, 18, 19, 20,
                  19, 20, 21, 22, 23, 24,
                  23, 24, 25, 26, 27, 28,
                  27, 28, 29, 30, 31,  0
  };

  uint64_t e = 0;

  for (int i = 0; i < 48; i += 1) {
    e <<= 1;
    e |= 0x01 & (x >> (31 - ep[i]));
  }

  return e;
}

uint64_t F(uint32_t R, uint64_t K) {
  uint64_t eR = E(R);
  uint64_t eR_xor_K = eR ^ K;

  std::cout << "\t\tE( " << std::hex << R << " ) :: " << eR << "\n";
  std::cout << "\t\t" << eR << " ^ " << K << " :: " << eR_xor_K << "\n"; 

  const uint8_t RMASK = 0x21;
  const uint8_t CMASK = 0x1e;

  uint32_t sbox_res = 0;
  for (uint8_t i = 0; i < 8; i += 1) {
    // get bit 0 and 5 from each block of eR_xor_K
    uint8_t block = (uint8_t) ((eR_xor_K >> 42 - (i * 6)));
    uint8_t row = (uint8_t) (RMASK & block);
    row = ((0x20 & row) >> 4) | (0x01 & row);
    uint8_t col = (uint8_t) (CMASK & block);
    col = col >> 1;

    sbox_res <<= 4;
    sbox_res |= 0xf & sboxes[i][row * 16 + col];
  }

  // permute with p table
  uint32_t res = 0;
  for (int i = 0; i < 32; i += 1) {
    res <<= 1;
    res |= 0x01 & (sbox_res >> (32 - p[i]));
  }

  std::cout << "\t\tF( " << R << ", " << K << " ) :: " << res << std::dec << "\n";

  return res;
}

uint64_t* key_gen(uint64_t K) {
  uint64_t* keys = (uint64_t*)malloc(sizeof(uint64_t) * 16);

  uint8_t K_shift_schedule[] = {
                                1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
  };

  std::cout << "Generating keys for 16 rounds...\n";

  uint64_t sK = 0;
  uint32_t C = 0, D = 0;
  for (uint8_t i = 0; i < 56; i += 1) {
    sK <<= 1;
    sK |= 0x01 & (K >> 64 - pc1[i]);
  }

  C = (uint32_t)(0x000000000fffffff & (sK >> 28));
  D = (uint32_t)(0x000000000fffffff & sK);

  // generate keys
  for (uint8_t round = 0; round < 16; round += 1) {
    // permute key with PC1
    std::cout << "|- keygen round " << std::dec << round + 1 << "\n";
    {
      uint8_t shift = K_shift_schedule[round];
      C = 0x0fffffff & ( (C << shift) | (C >> 28 - shift) );
      D = 0x0fffffff & ( (D << shift) | (D >> 28 - shift) );

      uint64_t subK = 0;
      subK = ((uint64_t)C << 28) | (uint64_t)(D);

      // PC2
      for (uint64_t i = 0; i < 48; i += 1) {
        keys[round] <<= 1;
        keys[round] |= 0x01 & (subK >> (56 - pc2[i]));
      }

      std::cout << std::hex <<
                   "   |- C    :: " << C << "\n" <<
                   "   |- D    :: " << D << "\n" <<
                   "   |- subK :: " << subK << "\n";
    }

    std::cout <<   "   |- key  :: " << std::hex << keys[round] << "\n";
  }

  std::cout << std::dec << "\n";
  return keys;
}

uint64_t des_encrypt(uint64_t P, uint64_t* Ks) {
  uint64_t cipher = 0;

  // initial permutation
  uint64_t permute_P = 0;
  for (uint64_t i = 0; i < 64; i += 1) {
    permute_P <<= 1;
    permute_P |= 0x01 & (P >> (64 - ip[i]));
  }

  uint32_t L = (uint32_t)(permute_P >> 32);
  uint32_t R = (uint32_t)(0x00000000ffffffff & permute_P);

  std::cout << "Initial L and R values...\n";
  std::cout << "L0 :: " << std::hex << L << "\n";
  std::cout << "R0 :: " << std::hex << R << std::dec << "\n\n";

  for (uint32_t i = 0; i < 16; i += 1) {
    std::cout << "Round " << i + 1 << "\n";

    uint32_t L_ = R;
    uint32_t R_ = L ^ F(R, Ks[i]);

    std::cout << "\tL" << i + 1 << " :: " << std::hex << L_ << std::dec <<  "\n";
    std::cout << "\tR" << i + 1 << " :: " << std::hex << R_ << std::dec <<  "\n";

    L = L_;
    R = R_;
  }
  std::cout << "\n";

  uint32_t tmp = L;
  L = R;
  R = tmp;

  cipher = ((uint64_t)L << 32) | (uint64_t)R;
  uint64_t fp_cipher = 0;
  for (uint8_t i = 0; i < 64; i += 1) {
    fp_cipher <<= 1;
    fp_cipher |= 0x01 & (cipher >> (64 - fp[i]));
  }

  return fp_cipher;
}

int main() {

  uint64_t P = 0x0123456789abcdef;
  uint64_t K = 0x0123456789abcdef;

  std::cout << "Starting Plaintext :: " << std::hex << P << "\n" <<
    "Starting Key       :: " << std::hex << K << "\n";

  uint64_t* Ks = key_gen(K);

  uint64_t C = des_encrypt(P, Ks);

  std::cout << "Ending Ciphertext  :: " << std::hex << C << "\n";

  return 0;
}
