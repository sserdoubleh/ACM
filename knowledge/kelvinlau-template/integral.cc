#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;

/* simpson integral of f at [a, b] */
double simpson(double (*f)(double), double a, double b) {
  int n = (int)(10000 * (b - a)); n -= n % 2;
  double A = 0, B = 0, d = (b - a) / n;
  for (int i = 1; i < n; i += 2)
    A += f(a + i * d);
  for (int i = 2; i < n; i += 2)
    B += f(a + i * d);
  return (f(a) + f(b) + 4 * A + 2 * B) * d / 3;
}

/* romberg integral of f at [a, b] */
double romberg(double (*f)(double), double l, double r) {
  const int N = 18;
  double a[N][N], p[N];

  p[0] = 1;
  for (int i = 1; i < N; i++)
    p[i] = p[i - 1] * 4;

  a[0][0] = (f(l) + f(r)) / 2;
  for (int i = 1, n = 2; i < N; i++, n <<= 1) {
    a[i][0] = 0;
    for (int j = 1; j < n; j += 2)
      a[i][0] += f((r - l) * j / n + l);
    a[i][0] += a[i - 1][0] * (n / 2);
    a[i][0] /= n;
  }
  for (int j = 1; j < N; j++)
    for (int i = 0; i < N - j; i++)
      a[i][j] = (a[i + 1][j - 1] * p[j] - a[i][j - 1]) / (p[j] - 1);
  return a[0][N - 1] * (r - l);
}

/* helper function of adaptive_simpsons */
double adaptive_simpsons_aux(double (*f)(double), double a, double b, double eps,
                         double s, double fa, double fb, double fc, int depth) {
  double c = (a + b) / 2, h = b - a;
  double d = (a + c) / 2, e = (c + b) / 2;
  double fd = f(d), fe = f(e);
  double sl = (fa + 4 * fd + fc) * h / 12;
  double sr = (fc + 4 * fe + fb) * h / 12;
  double s2 = sl + sr;
  if (depth <= 0 || fabs(s2 - s) <= 15 * eps)
    return s2 + (s2 - s) / 15;
  return adaptive_simpsons_aux(f, a, c, eps / 2, sl, fa, fc, fd, depth - 1) +
         adaptive_simpsons_aux(f, c, b, eps / 2, sr, fc, fb, fe, depth - 1);
}

/* Adaptive Simpson's Rule, integral of f at [a, b], max error of eps, max depth of depth */
double adaptive_simpsons(double (*f)(double), double a, double b, double eps, int depth) {
  double c = (a + b) / 2, h = b - a;
  double fa = f(a), fb = f(b), fc = f(c);
  double s = (fa + 4 * fc + fb) * h / 6;
  return adaptive_simpsons_aux(f, a, b, eps, s, fa, fb, fc, depth);
}
