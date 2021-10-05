#include <iostream>
#include <iomanip>
#include <string.h>

int mod(int x, int y) {
  if (x < 0) {
    while (x < 0) x += y;
    return x;
  } else {
    return x % y;
  }
}

char* vigenere_encrypt(const char* p, const char* k) {
  int p_size = (int)strlen(p),
      k_size = (int)strlen(k);

  char* c = (char*)malloc(sizeof(p) * p_size + 1);
  c[p_size] = 0;

  for (size_t i = 0; i < p_size; i += 1) {
    c[i] = mod(((p[i] - 'a') + (k[i % k_size] - 'a')), 26) + 'A';
  }

  return c;
}

char* vigenere_decrypt(const char* c, const char* k) {
  int c_size = (int)strlen(c),
      k_size = (int)strlen(k);

  char* p = (char*)malloc(sizeof(c) * c_size + 1);
  p[c_size] = 0;

  for (size_t i = 0; i < c_size; i += 1) {
    p[i] = mod(((c[i] - 'A') - (k[i % k_size] - 'a')), 26) + 'a';
  }

  return p;
}

int main() {
  const char* p = "explanation";
  const char* k = "leg";

  const char* c = vigenere_encrypt(p, k);
  std::cout << "plaintext  = " << p << "\n";
  std::cout << "key        = " << k << "\n";
  std::cout << "ciphertext = " << c << "\n";
  std::cout << "check      = " << vigenere_decrypt(c, k) << "\n";

  return 0;
}
