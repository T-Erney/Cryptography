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
                             (i + 1 < p_size) ? P[i + 1] - 'a' : 0,
                             (i + 2 < p_size) ? P[i + 2] - 'a' : 0,
                             (i + 3 < p_size) ? P[i + 3] - 'a' : 0);
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

char* hillcipher_decrypt(char* C, Mat22* inv_K) {
  int c_size = (int)strlen(C);
  char* P = (char*)malloc(sizeof(char)* c_size + 1);
  P[c_size] = 0;

  // add padding to C
  int pad = 0;
  while ((c_size + pad) % 4 != 0) {
    pad += 1;
    C = (char*)realloc(C, c_size + pad);
    C[c_size + pad - 1] = ' ';
  }

  // decode P = C*K^(-1)
  int i = 0;
  int j = 0;
  for (; i < c_size; i += 4) {
    Mat22* c_mat = mat22_set((i + 0 < c_size) ? C[i + 0] - 'A' : 0,
                             (i + 1 < c_size) ? C[i + 1] - 'A' : 0,
                             (i + 2 < c_size) ? C[i + 2] - 'A' : 0,
                             (i + 3 < c_size) ? C[i + 3] - 'A' : 0);

    Mat22* p_mat = mat22_mul(c_mat, inv_K);

    if (i + 0 < c_size) P[j++] = p_mat->data[0][0] + 'a';
    if (i + 1 < c_size) P[j++] = p_mat->data[0][1] + 'a';
    if (i + 2 < c_size) P[j++] = p_mat->data[1][0] + 'a';
    if (i + 3 < c_size) P[j++] = p_mat->data[1][1] + 'a';

    free(c_mat);
    free(p_mat);
  }

  return P;
}


int main() {

  {
    Mat22* K = mat22_set(9, 4, 5, 7);
    char P[] = "meetmeattheusualplaceattenratherthaneightoclock";
    char* C  = hillcipher_encrypt(P, K);

    std::cout << "Plaintext  :: " << P << "\n";
    std::cout << "Ciphertext :: " << C << "\n\n";

    free(K);
    free(C);
  }

  std::cout << "--------------------------------------------------\n\n";

  {
    Mat22* K = mat22_set(9, 13, 2, 3);
    char* C  = (char*)malloc(sizeof(char) * 6); // this needs to be heap allocated so we can manipulate it in decrypt
    memcpy(C, "YIFZMA", 6);
    
    Mat22* inv_K = mat22_inv(K);
    Mat22* identity = mat22_mul(K, inv_K);
    char* P = hillcipher_decrypt(C, inv_K);

    std::cout << "Ciphertext :: " << C << "\n";
    std::cout << "Plaintext  :: " << P << "\n\n";

    free(inv_K);
    free(K);
    free(P);
  }

  std::cout << "--------------------------------------------------\n\n";

  {
    // known plaintext attack
    char P[] = "howareyoutoday";
    char C[] = "ZWSENIUSPLJVEU";

    std::cout << "Plaintext  :: " << P << "\n";
    std::cout << "Ciphertext :: " << C << "\n\n";

    // load p_mat and see if it is invertible
    Mat22* p_mat = mat22_set(P[0] - 'a', P[1] - 'a', P[8] - 'a', P[9] - 'a');
    Mat22* inv_p_mat = mat22_inv(p_mat);
    Mat22* identity_of_p = mat22_mul(p_mat, inv_p_mat);

    std::cout << "Using " << P[0] << P[1] << " and " << P[8] << P[9] << " from the plaintext, we get the following...\n\n";

    std::cout << "p_mat :: \n";
    mat22_print(p_mat);
    std::cout << "\n";

    std::cout << "inv_p_mat :: \n";
    mat22_print(inv_p_mat);
    std::cout << "\n";

    std::cout << "identity_of_p :: \n";
    mat22_print(identity_of_p);
    std::cout << "\n";

    // since inv_p is invertible, we will use inv_p_mat * c_mat to get K
    // build a c_mat
    Mat22* c_mat = mat22_set(C[0] - 'A', C[1] - 'A', C[8] - 'A', C[9] - 'A');
    Mat22* K = mat22_mul(inv_p_mat, c_mat);
    
    std::cout << "Using " << C[0] << C[1] << " and " << C[8] << C[9] << " from the ciphertext, multiplied with the inverse of p_mat, we get the following...\n\n";

    std::cout << "K :: \n";
    mat22_print(K);
    std::cout << "\n";
  }
  return 0;
}
