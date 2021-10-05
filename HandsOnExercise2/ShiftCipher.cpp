#include <string>
#include <iostream>
#include <iomanip>

// since C/C++ modulus doesn't handle negatives well, I have implemented a modulus function
// such that negative numbers will be handled appropriately.
int mod(int b, int m) {
  if (b < 0) {
    while (b < 0) b += m;
    return b;
  }

  return b % m;
} 

std::string shift_encrypt(std::string, int);
std::string shift_decrypt(std::string, int);
void        shift_brute_force(std::string);

int main() {
  std::string plaintext = "attack";
  std::string ciphertext = " ";
  int key = 10;

  ciphertext = shift_encrypt(plaintext, key);
  std::cout << "1. Encryption of the plaintext: " << plaintext << "\n";
  std::cout << "   Key = 10\n";
  std::cout << "   Ciphertext = " << ciphertext << "\n";

  plaintext = shift_decrypt(ciphertext, key);
  std::cout << "2. Decryption of the ciphertext: " << ciphertext << "\n";
  std::cout << "   Key = " << key << "\n";
  std::cout << "   Plaintext = " << plaintext << "\n";

  ciphertext = "YCVEJQWVHQTDTWVWU";
  std::cout << "3. Brute Force Decrypt with Shift Cipher\n";
  std::cout << "   Decryption of the ciphertext: " << ciphertext << "\n";
  shift_brute_force(ciphertext);

  return 0;
}

// encrypt the given plaintext with the provided shift key.
std::string shift_encrypt(std::string plaintext, int shift) {
  std::string e = "";

  for (char c: plaintext) {
    // c - 'a' will give us the value of the character in the range of [0..25] which is what we want.
    // we then add our shift to the value found above.
    // modulus by 26 to bring back the value to the range of [0..25] in case we exceeded it.
    // we then finish off by adding 'A' back on to get our character in the range of ['A'..'Z'].
    e += mod(((c - 'a') + shift), 26) + 'A';
  }

  return e;
}

// decrypt the given ciphertext with the provided shift key.
std::string shift_decrypt(std::string ciphertext, int shift) {
  std::string p = "";

  for (char c: ciphertext) {
    // c - 'A' will give us the value of the character in the range of [0..25] which is what we want.
    // we then subtract our shift to the value found above.
    // modulus by 26 to bring the value back to the range of [0..25] in case we exceed it (or go negative).
    // we then finish off by adding 'a' back on to get our character in the range of ['a'..'z'].
    p += mod(((c - 'A') - shift), 26) + 'a';
  }

  return p;
}

// decrypt the cyphertext using all keys and display the new plaintext and key associated with it.
void shift_brute_force(std::string ciphertext) {
  for (int key = 1; key < 26; key += 1) {
    std::cout << "Shift Cipher Decryption with key := " << key << " : ";
    std::string p = shift_decrypt(ciphertext, key);
    std::cout << p << "\n";
  }
}
