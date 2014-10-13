#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int M = 100;
const int R = 10001;
typedef int matrix[M][M];

/* A = d * I */
void init(int m, matrix A, int d) {
  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++)
      A[i][j] = i == j ? d : 0;
}

/* C = A * B */
void mul(int m, matrix C, matrix A, matrix B) {
  static matrix T;
  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++) {
      T[i][j] = 0;
      for (int k = 0; k < m; k++)
        T[i][j] = (T[i][j] + A[i][k] * B[k][j]) % R;
    }
  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++)
      C[i][j] = T[i][j];
}

/* B = A ^ k */
void pow(int m, matrix B, matrix A, int k) {
  for (init(m, B, 1); k; k >>= 1) {
    if (k & 1) mul(m, B, A, B);
    mul(m, A, A, A);
  }
}

void print(int m, matrix A) {
  for (int i = 0; i < m; i++, puts(""))
    for (int j = 0; j < m; j++)
      printf("%d ", A[i][j]);
  puts("");
}

/* C = A + B */
void add(int m, matrix C, matrix A, matrix B) {
  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++)
      C[i][j] = (A[i][j] + B[i][j]) % R;
}

/* D = I + A + A ^ 2 + ... + A ^ (k - 1) */
void pow_sig(int m, matrix D, matrix A, int k) { 
  static matrix B, C, T;
  for (init(m, B, 1), init(m, C, 1), init(m, D, 0); k; k >>= 1) {
    if (k & 1) {
      mul(m, T, B, C); // D = D + C * B
      add(m, D, T, D);
      mul(m, B, A, B); // B = A * B
    }
    mul(m, T, A, C); // C = C + C * A
    add(m, C, T, C);
    mul(m, A, A, A); // A = A * A
  }
}

int main() {
  int n; 
  while (~scanf("%d", &n)) {
    matrix A = {
      { 0, 1 },
      { 1, 1 }
    };
    matrix B;
    pow(2, B, A, n);
    printf("%d\n", B[1][0]);
  }
  return 0;
}
