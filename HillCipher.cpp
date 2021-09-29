#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>

int mod(int x, int m) {
  if (x < 0) {
    while (x < 0) x += m;
    return x;
  } else {
    return x % m;
  }
}

int mult_inv(int i) {
  int k = 0;
  int j = 0;
  for (j = 0; j < 26; j += 1) {
    k = (i * j);
    if (k % 26 == 1) break;
  }
  return j;
}

struct Mat22 {
  int data[2][2];
};

Mat22* mat22_set(int a, int b, int c, int d) {
  Mat22* m = (Mat22*)malloc(sizeof(Mat22));
  m->data[0][0] = a;
  m->data[0][1] = b;
  m->data[1][0] = c;
  m->data[1][1] = d;
  return m;
}

void mat22_print(Mat22* m) {
  std::cout  << "|" << std::setw(2) << (m->data[0][0]) << " " << std::setw(2) << (m->data[0][1]) << "|";
  std::cout  << "\n";
  std::cout  << "|" << std::setw(2) << (m->data[1][0]) << " " << std::setw(2) << (m->data[1][1]) << "|";
  std::cout  << "\n";
}

Mat22* mat22_mul(Mat22* m, Mat22* n) {
  Mat22* o = (Mat22*)malloc(sizeof(Mat22));

  o->data[0][0] = mod((m->data[0][0] * n->data[0][0]) + (m->data[0][1] * n->data[1][0]), 26);
  o->data[0][1] = mod((m->data[0][0] * n->data[0][1]) + (m->data[0][1] * n->data[1][1]), 26);
  o->data[1][0] = mod((m->data[1][0] * n->data[0][0]) + (m->data[1][1] * n->data[1][0]), 26);
  o->data[1][1] = mod((m->data[1][0] * n->data[0][1]) + (m->data[1][1] * n->data[1][1]), 26);

  return o;
}

Mat22* mat22_adj(Mat22* m) {
  Mat22* adj = (Mat22*)malloc(sizeof(Mat22));
  adj->data[0][0] = m->data[1][1];
  adj->data[0][1] = m->data[0][1] * -1;
  adj->data[1][0] = m->data[1][0] * -1;
  adj->data[1][1] = m->data[0][0];

  return adj;
}

// inv = (1/(ad - bc))[a1 a2]
//                    [b1 b2]
Mat22* mat22_inv(Mat22* m) {
  Mat22* inv = mat22_adj(m);

  int denom   = mod((m->data[0][0] * m->data[1][1]) - (m->data[0][1] * m->data[1][0]), 26);
  int inverse = mult_inv(denom);

  inv->data[0][0] = mod(inv->data[0][0] * inverse, 26);
  inv->data[0][1] = mod(inv->data[0][1] * inverse, 26);
  inv->data[1][0] = mod(inv->data[1][0] * inverse, 26);
  inv->data[1][1] = mod(inv->data[1][1] * inverse, 26);

  return inv;
}

// C = E(P, K) = P * K (mod 26)
char* hillcipher_encrypt(char* P, Mat22* K) {
  int p_size = (int)strlen(P);
  char* C = (char*)malloc(sizeof(char) * p_size + 1);
  C[p_size] = 0;

  // encode C = P*K
  int i = 0;
  int j = 0;
  for (; i < p_size; i += 4) {
    Mat22* p_mat = mat22_set(P[i + 0] - 'a',
                             P[i + 1] - 'a',
                             P[i + 2] - 'a',
                             P[i + 3] - 'a');
    Mat22* c_mat = mat22_mul(p_mat, K);

    if (i     < p_size) C[j++] = c_mat->data[0][0] + 'A';
    if (i + 1 < p_size) C[j++] = c_mat->data[0][1] + 'A';
    if (i + 2 < p_size) C[j++] = c_mat->data[1][0] + 'A';
    if (i + 3 < p_size) C[j++] = c_mat->data[1][1] + 'A';

    free(p_mat);
    free(c_mat);
  }

  return C;
}

char* hillcipher_decrypt(char* C, Mat22* K) {
  int c_size = (int)strlen(C);
  char* P = (char*)malloc(sizeof(char)* c_size + 1);
  P[c_size] = 0;

  // get inv_K
  Mat22* inv_K = mat22_inv(K);

  // decode P = C*K^(-1)
  int i = 0;
  int j = 0;
  for (; i < c_size; i += 4) {
    Mat22* c_mat = mat22_set(C[i + 0] - 'A',
                             C[i + 1] - 'A',
                             C[i + 2] - 'A',
                             C[i + 3] - 'A');
    Mat22* p_mat = mat22_mul(c_mat, inv_K);

    if (i     < c_size) P[j++] = c_mat->data[0][0] + 'a';
    if (i + 1 < c_size) P[j++] = c_mat->data[0][1] + 'a';
    if (i + 2 < c_size) P[j++] = c_mat->data[1][0] + 'a';
    if (i + 3 < c_size) P[j++] = c_mat->data[1][1] + 'a';

    free(c_mat);
    free(p_mat);
  }

  return P;
}

int main() {

  {
    Mat22* K = mat22_set(9, 4, 5, 7);
    Mat22* inv_K = mat22_inv(K);

    std::cout << "K :: \n";
    mat22_print(K);
    std::cout << "\n";

    std::cout << "K(-1) :: \n";
    mat22_print(inv_K);
    std::cout << "\n";

    // test if inverse
    Mat22* identity = mat22_mul(K, inv_K);
    std::cout << "Identity Matrix :: \n";
    mat22_print(identity);
    std::cout << "\n";

    free(K);
    free(inv_K);
    free(identity);
  }

  std::cout << "------------------------\n\n";

  {
    char* P = (char*)malloc(7);
    memcpy(P, "meetme", 6);
    std::cout << "Plaintext = " << P << "\n\n";

    Mat22* K = mat22_set(9, 4, 5, 7);
    std::cout << "Key = \n";
    mat22_print(K);
    std::cout << "\n";

    char* C = hillcipher_encrypt(P, K);
    std::cout << "Ciphertext = " << C << "\n\n";

    std::cout << "decryption of " << C << " = " << hillcipher_decrypt(C, K) << "\n";

    free(P);
    free(K);
    free(C);
  }

  std::cout << "------------------------\n\n";

  {
    char* C = (char*)malloc(7);
    memcpy(C, "RDJAYF", 6);
    std::cout << "Ciphertext = " << C << "\n\n";

    Mat22* K = mat22_set(9, 4, 5, 7);
    std::cout << "Key = \n";
    mat22_print(K);
    std::cout << "\n";

    char* P = hillcipher_decrypt(C, K);
    std::cout << "Plaintext = " << P << "\n\n";
  }

  std::cout << "------------------------\n\n";

  {
    char* P = "howareyoutoday";
    char* C = "ZWSENIUSPLJVEU";

    Mat22* K = NULL;

    Mat22* p_mat = mat22_set('h', 'o', 'w', 'a');
    Mat22* c_mat = mat22_set('Z', 'W', 'S', 'E');

    int denom = mod((p_mat->data[0][0] * p_mat->data[1][1]) - (p_mat->data[0][1] * p_mat->data[1][0]), 26);

    std::cout << "If p_mat :: | h  o| then p_mat isn't invertible because the determinant of p_mat is " << denom << "\n";
    std::cout << "            | w  a|\n";
  }

  return 0;
}
