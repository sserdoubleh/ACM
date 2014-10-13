#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
using namespace std;

const int M = 4;

typedef double T;
typedef T matrix[M][M];
typedef T vector[M];

const T eps = 1e-4, pi = acos(-1.0);

inline int sign(T x) { 
  return x < -eps ? -1 : x > eps; 
}

inline T sqr(T x) {
  return x * x;
}

inline T frand() {
  return rand() / (RAND_MAX + 1.0);
}

void init(matrix A, int n, T d) {
  memset(A, 0, sizeof(A));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      A[i][j] = (i == j ? d : 0);
}

void mul(matrix A, matrix B, matrix C, int n) {
  static matrix E;
  memset(E, 0, sizeof(E));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      E[i][j] = 0;
      for (int k = 0; k < n; k++)
        E[i][j] += A[i][k] * B[k][j];
    }
  memcpy(C, E, sizeof(matrix));
}

void pow(matrix A, int k, int n) {
  static matrix E;
  init(E, n, 1);
  while (k) {
    if (k & 1)
      mul(A, E, E, n);
    mul(A, A, A, n);
    k >>= 1;
  }
  memcpy(A, E, sizeof(matrix));
}

void mul_matrix_vector(matrix A, vector x, vector y, int n) {
  static vector z;
  memset(z, 0, sizeof(z));
  for (int i = 0; i < n; i++) {
    z[i] = 0;
    for (int j = 0; j < n; j++)
      z[i] += x[j] * A[i][j];
  }
  memcpy(y, z, sizeof(vector));
}

void print(matrix A, int n) {
  for (int i = 0; i < n; i++, puts(""))
    for (int j = 0; j < n; j++)
      printf("%7.3lf", A[i][j]);
  puts("");
}

typedef struct point {
  T x, y, z;
} vt;

vt operator+(point a, point b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

vt operator-(point a, point b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}

vt operator*(T t, vt a) {
  a.x *= t;
  a.y *= t;
  a.z *= t;
  return a;
}

vt operator*(vt a, T t) {
  a.x *= t;
  a.y *= t;
  a.z *= t;
  return a;
}

vt operator/(vt a, T t) {
  a.x /= t;
  a.y /= t;
  a.z /= t;
  return a;
}

vt operator*(vt a, vt b) {
  vt c;
  c.x = a.y * b.z - a.z * b.y;
  c.y = a.z * b.x - a.x * b.z;
  c.z = a.x * b.y - a.y * b.x;
  return c;
}

T operator^(vt a, vt b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

T len(vt a) {
  return sqrt(a ^ a);
}

T len2(vt a) {
  return a ^ a;
}

void regularize(point *p) {
  T l = len(*p);
  p->x /= l;
  p->y /= l;
  p->z /= l;
}

int zero(vt a) {
  return !sign(a.x) && !sign(a.y) && !sign(a.z);
}

vt perp(vt p) {
  vt q, v;
  while (1) {
    q.x = frand();
    q.y = frand();
    q.z = frand();
    if (!zero(v = p * q)) return v;
  }
}

point transform(matrix A, point p) {
  vector vec = { p.x, p.y, p.z, 1.0 };
  mul_matrix_vector(A, vec, vec, M);
  p.x = vec[0];
  p.y = vec[1];
  p.z = vec[2];
  return p;
}

int main() {
  T x, y, z;
  T s, c, t, theta;
  vt vx, vy, vz;
  point p;
  char cmd[100];
  int n;
  matrix A;

  while (scanf("%d", &n), n) {

    init(A, M, 1);
    while (scanf("%s", cmd) == 1) {

      if (!strcmp(cmd, "translate")) {
        scanf("%lf %lf %lf", &x, &y, &z);
        matrix B = {
          { 0, 0, 0, x },
          { 0, 0, 0, y },
          { 0, 0, 0, z },
          { 0, 0, 0, 1 },
        };
        mul(B, A, A, M);

      } else if (!strcmp(cmd, "scale")) {
        scanf("%lf %lf %lf", &x, &y, &z);
        matrix B = {
          { x, 0, 0, 0 },
          { 0, y, 0, 0 },
          { 0, 0, z, 0 },
          { 0, 0, 0, 1 },
        };
        mul(B, A, A, M);

      } else if (!strcmp(cmd, "rotate_to_z")) {
        scanf("%lf %lf %lf", &vz.x, &vz.y, &vz.z);
        regularize(&vz);
        vx = perp(vz);
        regularize(&vx);
        vy = vx * vz;

        matrix B = {
          { vx.x, vx.y, vx.z, 0.0 },
          { vy.x, vy.y, vy.z, 0.0 },
          { vz.x, vz.y, vz.z, 0.0 },
          { 0.0,  0.0,  0.0,  1.0 },
        };
        mul(B, A, A, M);

      } else if (!strcmp(cmd, "rotate_about_axis")) {
        scanf("%lf %lf %lf %lf", &x, &y, &z, &theta);
        t = sqrt(sqr(x) + sqr(y) + sqr(z));
        x /= t, y /= t, z /= t;
        theta *= pi / 180.0;
        s = sin(theta);
        c = cos(theta);
        t = 1 - c;
        matrix B = {
          { x * x * t + c,     x * y * t - z * s, x * z * t + y * s, 0.0 },
          { y * x * t + z * s, y * y * t + c,     y * z * t - x * s, 0.0 },
          { z * x * t - y * s, z * y * t + x * s, z * z * t + c,     0.0 },
          { 0.0,               0.0,               0.0,               1.0 }
        };
        mul(B, A, A, M);
      }
    }

    for (int i = 0; i < n; i++) {
      scanf("%lf %lf %lf", &p.x, &p.y, &p.z);
      p = transform(A, p);
      if (!sign(p.x)) p.x = 0;
      if (!sign(p.y)) p.y = 0;
      if (!sign(p.z)) p.z = 0;
      printf("%.2lf %.2lf %.2lf\n", p.x, p.y, p.z);
    }
    puts("");
  }
  return 0;
}
