#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define swap(a, b) \
  {                \
    uint8_t tmp = a; \
    a = b;           \
    b = tmp;         \
  }

char* rc4(const char* P, char* K, size_t s) {
  char* C = (char*)malloc(strlen(P));

  uint8_t* S = (uint8_t*)malloc(sizeof(uint8_t) * s),
         * T = (uint8_t*)malloc(sizeof(uint8_t) * s);

  size_t i, j;
  for (i = 0; i < s; i += 1) {
    S[i] = i;
    T[i] = K[i % strlen(K)];
  }

  for (i = 0; i < s; i += 1) {
    j = (j + S[i] + T[i]) % s;
    swap(S[i], S[j]);
  }

  i = j = 0;
  for (int x = 0; x < (int)strlen(P); x += 1) {
    i = (i + 1) % s;
    j = (j + S[i]) % s;
    swap(S[i], S[j]);
    int t = (S[i] + S[j]) % s;
    int k = S[t];

    C[x] = P[x] ^ k;
  }

  return C;
}

int main() {
  char K[2]  = {1, 2};
  const char* P = "Hi";
  char* C = rc4(P, K, 4);

  std::cout << "P  :: " << P << "\n";
  std::cout << "C  :: " << C << "\n";
  std::cout << "P` :: " << rc4(C, K, 4) << "\n";

  return 0;
}
