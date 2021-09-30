#include <iostream>
#include <vector>
#include <string.h>


char** playfair_key_init(const char* K) {
  char c_table[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  bool b_table[26]; for (size_t i = 0; i < 26; i += 1) b_table[i] = false;

  char** pk = (char**)malloc(sizeof(char*) * 5);

  size_t K_size = strlen(K);
  size_t k = 0;
  size_t a = 0;
  for (size_t i = 0; i < 5; i += 1) {
    pk[i] = (char*)malloc(sizeof(char) * 5);
    for (size_t j = 0; j < 5; j += 1) {
      if (k < K_size) {
        pk[i][j] = K[k];
        b_table[K[k] - 'A'] = true;

        if (K[k] == 'i' || K[k] == 'j') {
          b_table[8] = true;
          b_table[9] = true;
        }

        k += 1;
      } else {
        if (a == 9) a += 1;
        if (b_table[a] == false) {
          pk[i][j] = c_table[a];
          b_table[a] = true;

          if (c_table[a] == 'i') {
            b_table[8] = true;
            b_table[9] = true;
          }

          a += 1;
        } else {
          a += 1;
          j -= 1;
        }
      }
    }
  }

  return pk;
}

// int pos[2] = { x, y }; where x == i and y == j
int* playfair_key_pos(const char c, char** pk) {
  int* pos = (int*)malloc(sizeof(int) * 2);
  pos[0] = 0;
  pos[1] = 0;

  for (size_t i = 0; i < 5; i += 1) {
    for (size_t j = 0; j < 5; j += 1) {
      if (c == pk[i][j]) {
        pos[0] = i;
        pos[1] = j;
      }
    }
  }

  return pos;
}

int main() {
  const char* K = "SCHMID";
  const char* P = "sellallstockonmonday";
  char*       C = (char*)malloc(sizeof(P) * strlen(P) + 1);
  C[strlen(P)] = 0;

  // ---------------------
  // Get key in 5x5 Matrix
  // ---------------------
  char** pk = playfair_key_init(K);
  for (int i = 0; i < 5; i += 1) {
    for (int j = 0; j < 5; j += 1) {
      std::cout << pk[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  // -------------------------
  // Generate chunks of size 2
  // -------------------------
  std::vector<char*> chunks; chunks.reserve(1);
  for (size_t i = 0; i < strlen(P);) {
    char x, y;
    x = P[i++] - 32;
    y = P[i++] - 32;

    if (x == y) {
      y = 'x' - 32;
      i -= 1;
    }

    char* pair = (char*)malloc(sizeof(char) * 2);
    pair[0] = x;
    pair[1] = y;
    chunks.push_back(pair);
  }

  // ------------
  // Encrypt Data
  // ------------
  std::vector<char*> enc_chunks; enc_chunks.reserve(1);
  for (size_t i = 0; i < chunks.size(); i += 1) {
    char x = chunks[i][0];
    char y = chunks[i][1];

    // find positions of x and y where they reside in pk
    int* xpos = playfair_key_pos(x, pk);
    int* ypos = playfair_key_pos(y, pk);

    // if they are in the same col
    if (xpos[0] == ypos[0]) {
      x = pk[xpos[0]][(xpos[1] + 1) % 5];
      y = pk[ypos[0]][(ypos[1] + 1) % 5];
    }
    // if they are in the same row
    else if (xpos[1] == ypos[1]) {
      x = pk[(xpos[0] + 1) % 5][xpos[1]];
      y = pk[(ypos[0] + 1) % 5][ypos[1]];
    }
    // otherwise
    else {
      x = pk[xpos[0]][ypos[1]];
      y = pk[ypos[0]][xpos[1]];
    }

    std::cout << x << y;

    char* pair = (char*)malloc(sizeof(char) * 2);
    pair[0] = x;
    pair[1] = y;
    enc_chunks.push_back(pair);
  }
  std::cout << "\n\n";

  // ----------------------------------------------
  // Rebuild enc_chunks to make a ciphertext string
  // ----------------------------------------------
  C = (char*)malloc(sizeof(char) * strlen(P) + 1);
  C[enc_chunks.size() * 2] = 0;

  for (size_t i = 0, j = 0; i < enc_chunks.size(); i += 1) {
    C[j++] = enc_chunks[i][0];
    C[j++] = enc_chunks[i][1];
  }

  std::cout << "Plaintext  = " << P << "\n";
  std::cout << "Ciphertext = " << C << "\n";

  return 0;
}
