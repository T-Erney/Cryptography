#include <iostream>
#include <string>

// since C/C++ modulus doesn't handle negatives will, I have implemented a modulus function
// in a way that handles negative modulus appropriately.
int mod(int b, int m) {
  if (b < 0) {
    while (b < 0) b += m;
    return b;
  }

  return b % m;
}

std::string affine_encrypt(std::string, int, int);
std::string affine_decrypt(std::string, int, int);
int*        affine_known_plaintext(std::string, std::string);

// gcd table which is to be used in junction with the mod26_inverse_table to lookup inverses
// without having to calculate.
static int mod26_gcd_table[] = {
   1,  3,  5,  7, 
   9, 11, 15, 17, 
  19, 21, 23, 25
};

// mod26 inverses of the mod26_gcd_table
static int mod26_inverse_table[] = {
   1,  9, 21, 15, 
   3, 19,  7, 23, 
  11,  5, 17, 25
};

int main() {
  int a = 3,
      b = 2;

  std::string plaintext = "affine";
  std::string ciphertext = " ";
  
  ciphertext = affine_encrypt(plaintext, a, b);
  std::cout << "1. Encrypt Plaintext : " << plaintext << " with (" << a << "x + " << b << ") : \n";
  std::cout << "   Ciphertext = " << ciphertext << "\n";

  ciphertext = "CVVWPM";
  int inv = -1;
  for (int i = 0; i < 12; i += 1) {
    if (mod26_gcd_table[i] == a) {
      inv = mod26_inverse_table[i];
      break;
    }
  }
  plaintext = affine_decrypt(ciphertext, inv, b);
  std::cout << "2. Decrypt Ciphertext : " << ciphertext << " with (x - " << b << ") * " << inv << " : \n";
  std::cout << "   Plaintext = " << plaintext << "\n";

  int* affine_alpha_beta = affine_known_plaintext("if", "BM");
  std::cout << "3. Find key with known Plaintext : if using BM : \n";
  std::cout << "   a = " << affine_alpha_beta[0] << "\n";
  std::cout << "   b = " << affine_alpha_beta[1] << "\n";
  std::cout << "   Check Work: affine_encrypt(if, " << 
    affine_alpha_beta[0] << 
    ", " << 
    affine_alpha_beta[1] << 
    ") = " <<
    affine_encrypt("if", affine_alpha_beta[0], affine_alpha_beta[1]) << "\n";
  std::cout << "   Key = (" << affine_alpha_beta[0] << "x + " << affine_alpha_beta[1] << ")\n";

  return 0;
}

// affine encryption of plaintext which uses the function (ax + b) to encrypt plaintext.
std::string affine_encrypt(std::string plaintext, int a, int b) {
  std::string e = "";

  for (char c: plaintext) {
    int x = (c - 'a');
    int y = (a * x) + b;
    e += mod(y, 26) + 'A';
  }

  return e;
}

// affine decryption of ciphertext which uses the function y = (x - b) * inv(a)
std::string affine_decrypt(std::string ciphertext, int inv, int b) {
  std::string p = "";

  for (char c: ciphertext) {
    int x = (c - 'A');
    int y = (x - b) * inv;
    p += mod(y, 26) + 'a';
  }

  return p;
}

// known plaintext decypher which uses a known plaintext and ciphertext pair to solve for 
// a and b.
int* affine_known_plaintext(std::string plaintext, std::string ciphertext) {
  int alpha, beta;
  
  // get the base values of ciphertext by subtracting 'A' from both.
  int y = mod(((ciphertext[0] - 'A') - (ciphertext[1] - 'A')), 26);
  // get the base value of plaintext by subtracting 'a' from both.
  int x = ((plaintext[0]  - 'a') - (plaintext[1]  - 'a'));

  // find alpha first
  // get inverse from inverse_table
  int inv;
  for (int i = 0; i < 12; i += 1) {
    if (mod26_gcd_table[i] == x) {
      inv = mod26_inverse_table[i];
      break;
    }
  }
  
  alpha = mod(inv * y, 26);

  // find beta
  beta = mod((ciphertext[0] - 'A') - (alpha * (plaintext[0] - 'a')), 26);

  int* ret = (int*)malloc(sizeof(int) * 2);
  ret[0] = alpha;
  ret[1] = beta;
  return ret;
}


