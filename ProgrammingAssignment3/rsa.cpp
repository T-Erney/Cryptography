/*
 * Tristan Erney
 * December 7th, 2021
 * Intro to Cryptology
 * Programming Assignment 3 - RSA
 */

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>

#define MAX_PRIME 1000
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

// so just a fast/fancy power function?
int fast_exponentiation(int b, int p, int m) {
  int a = 1;
  if (p == 0)          return a;
  else if (p % 2 == 0) a = mod(fast_exponentiation(b, p / 2, m) * fast_exponentiation(b, p / 2, m), m);
  else                 a = mod(b * fast_exponentiation(b, (p - 1) / 2, m) * fast_exponentiation(b, (p - 1) / 2, m), m);
  return a;
}

// realized I wasn't getting primes all the time
// from the prime_sieve so I implemented this to
// make sure I got primes and not "potential" primes
int fermat_primality_test(int n) {
  return fast_exponentiation(2, n - 1, n);
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
  srand(time(NULL) + rc++);
  int i = rand() % array_size;
  int j = rand() % 8;
  int k = primes[i][j];

  if (k != 0 && k > 100 && fermat_primality_test(k) == 1) return k;
  else return rand_prime(primes);
}

int gcd(int a, int b) {
  int r = 0;
  while (a > 1) {
    r = b - a * floor((float)(b / a));
    b = a;
    a = r;
  }
  return r;
}

int extended_euclidean(int a, int b) {
  int s = 0, s1 = 1;
  int r = b, r1 = a;
  int t = 1, t1 = 0;

  while (r != 0) {
    int q = r1 / r;
    int p;

    p = r;
    r = r1 - q * p;
    r1 = p;

    p = s;
    s = s1 - q * p;
    s1 = p;

    p = t;
    t = t1 - q * p;
    t1 = p;
  }

  return s1;
}

int rsa_enc(int n, int e, int m) {
  return mod(fast_exponentiation(m, e, n), n);
}

int rsa_dec(int n, int d, int C) {
  return mod(fast_exponentiation(C, d, n), n);
}

int32_t main() {
  int** primes = prime_sieve(MAX_PRIME);

  //int p = rand_prime(primes);
  //int q = rand_prime(primes);

  { // this is our first example
    int p = 181;
    int q = 101;
    int n = (p * q);
    int phi = (p - 1) * (q - 1);
    int e = 19;
    int d = mod(extended_euclidean(e, phi), phi);

    std::cout << "p :: " << p << "\n";
    std::cout << "q :: " << q << "\n";
    std::cout << "n :: " << n << "\n";
    std::cout << "phi :: " << phi << "\n";
    std::cout << "e :: " << e << "\n";
    std::cout << "d :: " << d << "\n";

    int m = 100;
    int C = rsa_enc(n, e, m);
    int P = rsa_dec(n, d, C);

    std::cout << "C :: " << C << "\n";
    std::cout << "m :: " << P << "\n\n";
  }

  {
    int p = rand_prime(primes);
    int q = rand_prime(primes);
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e;

    { // check gcd(phi, e) == 1
      int g = 0;
      while (g != 1) {
        e = rand_prime(primes);
        g = gcd(e, phi);
      }
    }

    int d = mod(extended_euclidean(e, phi), phi);

    std::cout << "p :: " << p << "\n";
    std::cout << "q :: " << q << "\n";
    std::cout << "n :: " << n << "\n";
    std::cout << "phi :: " << phi << "\n";
    std::cout << "e :: " << e << "\n";
    std::cout << "d :: " << d << "\n";

    int m = 100;
    int C = rsa_enc(n, e, m);
    int P = rsa_dec(n, d, C);

    std::cout << "C :: " << C << "\n";
    std::cout << "m :: " << P << "\n";

  }

  delete [] primes;
  return 0;
}
