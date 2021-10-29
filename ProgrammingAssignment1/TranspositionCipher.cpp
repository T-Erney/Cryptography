#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>

// expand string by adding 'x''s to the end if needed
char* expand_string(char* str, int desired_size) {
  int str_size = (int)strlen(str);
  char* s = (char*)malloc(sizeof(char) * desired_size + 1);
  s[desired_size] = 0;

  for (int i = 0; i < desired_size; i += 1) {
    s[i] = (i < str_size) ? str[i] : /* (('x' - 'a' + i) % 26) + 'a' + 1 */ 'x';
  }

  return s;
}

/*
  Row Transposition Encryption:

  encrypt by building a table of (k_size x (c_size / k_size))
  and rebuild a new ciphertext string by reading the table
  by column.

  meetmeaftertogaparty

  key = {3, 5, 1, 6, 2, 4}
  TBL:
  m e e t m e
  a f t e r t
  o g a p a r
  t y x y z w
  -----------
  3 5 1 6 2 4

  etaxmrazmaotetrwefgytepy
*/
char* row_transposition_encrypt(char* _P, int* K, int k_size) {
  int p_size = strlen(_P);
  int pad    = ((p_size % k_size) != 0) ? k_size - (p_size % k_size) : 0;
  int c_size = p_size + pad;

  char* P = expand_string(_P, c_size);
  char* C = (char*)malloc(c_size + 1);
  C[c_size] = 0;

  char** TBL = (char**)malloc(sizeof(char*) * (c_size / k_size));
  for (int i = 0; i < (c_size / k_size); i += 1) {
    char* p_ptr = P + (i * k_size);
    TBL[i] = (char*)malloc(sizeof(char) * k_size);
    for (int j = 0; j < k_size; j += 1) {
      TBL[i][j] = p_ptr[j];
    }
  }

  int k = 0;
  for (int i = 0; i < k_size; i += 1) {

    // get keys in order of 0 to k_size - 1
    int c = 0;
    for (int j = 0; j < k_size; j += 1) {
      if (K[j] == i + 1) {
        c = j;
        break;
      }
    }

    for (int j = 0; j < (c_size / k_size); j += 1) {
      C[k++] = TBL[j][c];
    }
  }

  return C;
}

/*
  Row Transposition Decrypt:

  decrypt by building a table of (k_size x (c_size / k_size))
  by reading in the table by each (c_size / k_size) sized chunk
  ordered by the key, which is then read into a new plaintext
  string by reading the table left-right top-bottom.

  key = {3, 5, 1, 6, 2, 4}

  #3   #5   #1   #6   #2   #4
  amrt moep eaeg rtfy tzty xawe

  e t a x m r
  a z m a o t
  e t r w e f
  g y t e p y

  etaxmrazmaotetrwefgytepy
*/
char* row_transposition_decrypt(char* C, int* K, int k_size) {
  int c_size = strlen(C);
  int p_size = c_size;

  char* P = (char*)malloc(p_size + 1);
  P[p_size] = 0;

  // create table
  char** TBL = (char**)malloc(sizeof(char*) * (c_size / k_size));
  for (int i = 0; i < (c_size / k_size); i += 1) {
    char* c_ptr = C + (i * k_size);
    TBL[i] = (char*)malloc(sizeof(char) * k_size);
  }

  // read data into table
  for (int i = 0; i < k_size; i += 1) {
    char* c_ptr = C + (i * (c_size / k_size));
    for (int j = 0; j < (c_size / k_size); j += 1) {
      TBL[j][K[i] - 1] = c_ptr[j];
      //std::cout << TBL[j][K[i] - 1] << " ";
    }
    //std::cout << "\n";
  }
  //std::cout << "\n";

  int k = 0;
  for (int i = 0; i < (c_size / k_size); i += 1) {
    for (int j = 0; j < k_size; j += 1) {
      P[k++] = TBL[i][j];
    }
  }

  return P;
}

int main() {

  {
    int key[] = {4, 3, 1, 2, 5, 6, 7};
    char P[] = "attackpostponeduntiltwoam";
    char* C1 = row_transposition_encrypt(P, key, 7);
    char* C  = row_transposition_encrypt(C1, key, 7);

    std::cout << "Key        :: ";
    for (int i = 0; i < 7; i += 1) {
      std::cout << key[i] << " ";
    }
    std::cout << "\n";
    std::cout << "Plaintext  :: " << P << "\n";
    std::cout << "Ciphertext :: " << C << "\n\n";

  }

  {
    int key[] = {3, 5, 1, 6, 2, 4};
    char C[] = "amrtmoepeaegrtfytztyxawe";
    char* P  = row_transposition_decrypt(row_transposition_decrypt(C, key, 6), key, 6);

    std::cout << "Key        :: ";
    for (int i = 0; i < 6; i += 1) {
      std::cout << key[i] << " ";
    }
    std::cout << "\n";
    std::cout << "Ciphertext :: " << C << "\n";
    std::cout << "Plaintext  :: " << P << "\n\n";
  }

  return 0;
}
