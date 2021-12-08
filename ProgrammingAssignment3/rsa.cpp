#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>

#define MAX_PRIME 500
#define int int64_t
static int array_size = 0;

int mod(int x, int y) {
  if (x < 0) {
    while (x < 0) {
      x += y;
    }
    return x;
  } else {
    return x % y;
  }
}

int** prime_sieve(int end) {
  int J = floor(end/30) + 1;
  array_size = J;
  int** primes = new int*[J];
  for (int i = 0; i < J; i += 1) {
    primes[i] = new int[8];
    for (int j = 0; j < 8; j += 1) primes[i][j] = 0;
  }

  int K[] = { 1, 7, 11, 13, 17, 19, 23, 29 };
  for (int j = 0; j < floor(end / 30) + 1; j += 1) {
    for (int k = 0; k < 8; k += 1) {
      primes[j][k] = 30 * j + K[k];
    }
  }

  for (int j = 0; j < floor(end / 30) + 1; j += 1) {
    for (int k = 0; k < 8; k += 1) {
      int l = primes[j][k];
      if (l > 2 && l % 2 == 0) primes[j][k] = 0;
      if (l > 3 && l % 3 == 0) primes[j][k] = 0;
      if (l > 5 && l % 5 == 0) primes[j][k] = 0;
      if (l > 7 && l % 7 == 0) primes[j][k] = 0;
      if (l > 11 && l % 11 == 0) primes[j][k] = 0;
    }
  }

  return primes;
}

static int rc = 1;
int rand_prime(int* primes[8]) {
  srand(time(NULL) * rc++);
  int i = rand() % array_size;
  int j = rand() % 8;
  int k = primes[i][j];

  if (k != 0) return k;
  else return rand_prime(primes);
}

int eegcd(int a, int b, int* x, int* y) {
  if (a == 0) {
    *x = 0;
    *y = 1;
    return b;
  }

  int x1, y1;
  int gcd = eegcd(b % a, a, &x1, &y1);
  *x = y1 - (b / a) * x1;
  *y = x1;
  return gcd;
}

int power(int b, int e, int m) {
  int p = b;
  for (int i = 1; i < e; i += 1) p = mod(p * b, m);
  return p;
}

int rsa_enc(int p, int q, int e) {
  int n = p * q;
  int phi = (p - 1) * (q - 1);

  int x, y;
  //int gcd = eegcd()

  return 0;
}

int rsa_dec(int p, int q) {

  return 0;
}

int32_t main() {
  int** primes = prime_sieve(MAX_PRIME);

  //int p = rand_prime(primes);
  //int q = rand_prime(primes);
  int p = 181;
  int q = 101;
  int n = (p * q);
  int phi = (p - 1) * (q - 1);
  int e;

  { // check gcd(phi, e) == 1
    int gcd = 0;
    while (gcd != 1) {
      e = rand_prime(primes);
      int x, y;
      gcd = eegcd(phi, e, &x, &y);
    }
  }

  e = 19;

  int x, d;
  int gcd = eegcd(phi, e, &x, &d);
  d = mod(d, phi);

  std::cout << "p :: " << p << "\n";
  std::cout << "q :: " << q << "\n";
  std::cout << "n :: " << n << "\n";
  std::cout << "phi :: " << phi << "\n";
  std::cout << "e :: " << e << "\n";
  std::cout << "d :: " << d << "\n";

  int C = mod(power(100, e, n), n);
  int P = mod(power(C, d, n), n);

  std::cout << "C :: " << C << "\n";
  std::cout << "m :: " << P << "\n";

  delete [] primes;
  return 0;
}
