#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>

struct CipherFreqTable {
  char  letter[26];
  float freq[26];
};

int mod(int x, int y) {
  if (x < 0) {
    while (x < 0) x += y;
    return x;
  } else {
    return x % y;
  }
}

CipherFreqTable* letter_freq(const char* p) {
  CipherFreqTable* t = new CipherFreqTable;

  for (int i = 0; i < 26; i += 1) {
    t->letter[i] = 'A' + i;
    t->freq[i]   = 0;
  }

  for (int i = 0; i < strlen(p); i += 1) {
    int j = p[i] - 'A';
    t->freq[j] += 1;
  }

  return t;
}


char* shift_decrypt(const char* c, int k) {
  int c_size = (int)strlen(c);

  char* p = (char*)malloc(sizeof(c) * c_size + 1);
  p[c_size] = 0;

  for (size_t i = 0; i < c_size; i += 1) {
    p[i] = mod((c[i] - 'A') - k, 26) + 'a';
  }

  return p;
}


int main() {
  const char* ciphertext = "LCLLEWLJAZLNNZMVYIYLHRMHZA";

  // frequency analysis
  int k = -1;

  CipherFreqTable* freq_table = letter_freq(ciphertext);
  int  score  = 0;
  char letter = ' ';
  for (size_t j = 0; j < 26; j += 1) {
    if (score < freq_table->freq[j]) {
      score = freq_table->freq[j];
      letter = freq_table->letter[j];
    }
  }

  letter += 32;

  std::cout << "Most Frequent Letter :: " << letter << "\n";
  k = letter - 'e';

  char* plaintext = shift_decrypt(ciphertext, k);

  std::cout << "Key                  :: " << k << "\n";
  std::cout << "Plaintext            :: " << plaintext << "\n";

  return 0;
}

