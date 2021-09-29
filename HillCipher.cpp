#include <iostream>

int mod(int x, int m) {
  if (x < 0) {
    while (x < 0) x += m;
    return x;
  } else {
    return x % m;
  }
}

struct Mat22 {
  char data[2][2];
};

Mat22* mat22_set_key(int a1, int a2, int b1, int b2) {
  Mat22* m = (Mat22*)malloc(sizeof(Mat22));
  m->data[0][0] = a1;
  m->data[0][1] = a2;
  m->data[1][0] = b1;
  m->data[1][1] = b2;
  return m;
}

void mat22_print(Mat22* m) {
  std::cout  << "[" << (char)(m->data[0][0] + 'A') << " " << (char)(m->data[0][1] + 'A') << "]";
  std::cout  << "\n";
  std::cout  << "[" << (char)(m->data[1][0] + 'A') << " " << (char)(m->data[1][1] + 'A') << "]";
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

// inv = (1/(ad - bc))[a1 a2]
//                    [b1 b2]
Mat22* mat22_inv(Mat22* m) {
  Mat22* inv = (Mat22*)malloc(sizeof(Mat22));
  int denom = mod(((m->data[0][0] * m->data[1][1]) - (m->data[0][1] * m->data[1][0])), 26);
  inv->data[0][0] = mod(-(m->data[1][1]) * denom, 26);
  inv->data[0][1] = mod(m->data[1][0] * denom, 26);
  inv->data[1][0] = mod(m->data[0][1] * denom, 26);
  inv->data[1][1] = mod(-(m->data[0][0]) * denom, 26);
  return inv;
}

int main() {

  Mat22* K = mat22_set_key(5, 8, 17, 3);
  Mat22* inv_K = mat22_inv(K);

  std::cout << "K :: \n";
  mat22_print(K);

  std::cout << "\n" << "K(-1) :: \n";
  mat22_print(inv_K);

  // test if inverse
  Mat22* identity = mat22_mul(K, inv_K);
  std::cout << "Identity Matrix :: \n";
  mat22_print(identity);

  return 0;
}
