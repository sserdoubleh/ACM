#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
using namespace std;

const int N = 100010;
const double eps = 1e-8;

inline int sign(double x) { 
  return x < -eps ? -1 : x > eps; 
}

struct point { 
  double x, y; 
  point(double x = 0, double y = 0) : x(x), y(y) {}
};

struct line {
  point p, q;
  line() {}
  line(point p, point q) : p(p), q(q) {}
};

inline double cross(point a, point b, point c) {
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

inline int angle_cmp(line a, line b) {
  double a1 = atan2(a.q.y - a.p.y, a.q.x - a.p.x);
  double b1 = atan2(b.q.y - b.p.y, b.q.x - b.p.x);
  return sign(a1 - b1);
}

bool hp_cmp(line a, line b) {
  int d = angle_cmp(a, b);
  return d ? d < 0 : sign(cross(a.p, a.q, b.p)) < 0;
}

inline bool angle_equal(line a, line b) {
  return !angle_cmp(a, b);
}

inline bool in_hp(line l, point p) {
  return sign(cross(l.p, l.q, p)) >= 0;
}

point ip(line u, line v) {
  double n = (u.p.y - v.p.y) * (v.q.x - v.p.x) - (u.p.x - v.p.x) * (v.q.y - v.p.y);
  double d = (u.q.x - u.p.x) * (v.q.y - v.p.y) - (u.q.y - u.p.y) * (v.q.x - v.p.x);
  double r = n / d;
  return point(u.p.x + r * (u.q.x - u.p.x), u.p.y + r * (u.q.y - u.p.y));
}

void find_half_plane(line l[], int n, point p[], int &m) {
  static line *dq[N];
  static int s, t;

  sort(l, l + n, hp_cmp);
  n = unique(l, l + n, angle_equal) - l;

  s = t = 0;
  dq[t++] = &l[0];
  dq[t++] = &l[1];
  for (int i = 2; i < n; i++) {
    while (t - s > 1 && !in_hp(l[i], ip(*dq[t - 2], *dq[t - 1]))) t--;
    while (t - s > 1 && !in_hp(l[i], ip(*dq[s], *dq[s + 1]))) s++;
    dq[t++] = &l[i];
  }
  for (int c = 1; c;) {
    c = 0;
    while (t - s > 1 && !in_hp(*dq[t - 1], ip(*dq[s], *dq[s + 1]))) s++, c = 1;
    while (t - s > 1 && !in_hp(*dq[s], ip(*dq[t - 2], *dq[t - 1]))) t--, c = 1;
  }
  
  dq[t] = dq[s];
  for (int i = 0; i < t - s; i++) {
    p[i] = ip(*dq[s + i], *dq[s + i + 1]);
  }
  m = t - s;
}

int n, m;
line l[N];
point p[N];

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    point A, B;
    scanf("%lf %lf", &A.x, &A.y);
    scanf("%lf %lf", &B.x, &B.y);
    l[i] = line(A, B);
  }

  find_half_plane(l, n, p, m);
  p[m] = p[0];

  double area = 0;
  for (int i = 0; i < m; i++)
    area += p[i].x * p[i + 1].y - p[i].y * p[i + 1].x;
  printf("%lf\n", area / 2);

  return 0;
}
