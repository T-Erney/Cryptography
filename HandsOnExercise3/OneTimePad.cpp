#include <iostream>
#include <string.h>

int mod(int x, int y) {
  if (x < 0) {
    while (x < 0) x += y;
    return x;
  } else {
    return x % y;
  }
}

char* one_time_pad_decrypt(const char* c, const char* k) {
  int size = strlen(c);
  char* p = (char*)malloc(sizeof(c) * size + 1);
  p[size] = 0;

  for (int i = 0; i < size; i += 1) {
    p[i] = mod((c[i] - 'A') - (k[i] - 'a'), 26) + 'a';
  }

  return p;
}

int main() {

  const char* c  = "WOJDMNVHIC";
  const char* k1 = "pkysyhkdvp";
  const char* k2 = "rgszmuzzxr";

  const char* p1 = one_time_pad_decrypt(c, k1);
  const char* p2 = one_time_pad_decrypt(c, k2);

  std::cout << "p1 = one_time_pad_decrypt(" << c << ", " << k1 << ") := " << p1 << "\n";
  std::cout << "p2 = one_time_pad_decrypt(" << c << ", " << k2 << ") := " << p2 << "\n";
  return 0;
}
