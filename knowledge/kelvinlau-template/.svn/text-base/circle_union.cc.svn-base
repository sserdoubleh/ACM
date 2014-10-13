/* Given n circles and a rectangle, find the area of intersection of 
 * the rectangle and the union of these circles 
 * O(n ^ 3) */
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;

inline double sqr(double x) {
  return x * x;
}

const double eps = 1e-6;

inline int sign(double x) {
  return x < -eps ? -1 : x > eps;
}

inline bool deq(double x, double y) {
  return !sign(x - y);
}

struct point {
  double x, y;
};

struct circle {
  point o;
  double r;
};

inline int contains(const circle &a, const circle &b) {
  return a.r >= b.r &&
    sqr(a.o.x - b.o.x) + sqr(a.o.y - b.o.y) <= sqr(a.r - b.r);
}

inline int ip_circle_circle(const circle &c1, const circle &c2,
                            point &p1, point &p2) {
  double mx = c2.o.x - c1.o.x, sx = c2.o.x + c1.o.x, mx2 = sqr(mx);
  double my = c2.o.y - c1.o.y, sy = c2.o.y + c1.o.y, my2 = sqr(my);
  double sq = mx2 + my2, d = -(sq - sqr(c1.r - c2.r)) * (sq - sqr(c1.r + c2.r));
  if (!sign(sq)) return 0;
  if (d + eps < 0) return 0;
  if (d < eps) d = 0; else d = sqrt(d);
  double x = mx * ((c1.r + c2.r) * (c1.r - c2.r) + mx * sx) + sx * my2;
  double y = my * ((c1.r + c2.r) * (c1.r - c2.r) + my * sy) + sy * mx2;
  double dx = mx * d, dy = my * d; sq *= 2;
  p1.x = (x + dy) / sq; p1.y = (y - dx) / sq;
  p2.x = (x - dy) / sq; p2.y = (y + dx) / sq;
  return d > eps ? 2 : 1;
}

inline int ip_circle_y(const circle &c, double y, double &x1, double &x2) {
  double d = sqr(c.r) - sqr(y - c.o.y);
  if (sign(d) < 0) return 0;
  d = sign(d) ? d : 0;
  double dx = sqrt(d);
  x1 = c.o.x - dx;
  x2 = c.o.x + dx;
  return d > eps ? 2 : 1;
}

inline int ip_circle_x(const circle &c, double x, double &y1, double &y2) {
  double d = sqr(c.r) - sqr(x - c.o.x);
  if (sign(d) < 0) return 0;
  d = sign(d) ? d : 0;
  double dy = sqrt(d);
  y1 = c.o.y - dy;
  y2 = c.o.y + dy;
  return d > eps ? 2 : 1;
}

inline double area_bow(double r, double l) {
  double tri = l * sqrt(max(0.0, sqr(r) - sqr(l / 2))) / 2;
  double theta = 2 * asin(l / (r + r));
  double sector = theta * r * r / 2;
  return sector - tri;
}

struct arc {
  circle *c;
  double x1, x2;
  int t;
};

bool operator<(const arc &a, const arc &b) {
  return a.x1 + a.x2 < b.x1 + b.x2;
}

const int N = 200 + 10;

circle c[N]; 
int n, n1, n2;
double ys[N * N + N * 7 + 2];
int ysn;
double X, Y;
arc a[N * 2];
int an;

double solve(double ya, double yb) {
  an = 0;
  for (int i = 0; i < n; i++) {
    double x1, x2, x3, x4;
    if (ip_circle_y(c[i], ya, x1, x3) && ip_circle_y(c[i], yb, x2, x4)) {
      if (x1 + x2 > X + X || x3 + x4 < 0) continue;
      a[an++] = (arc) { &c[i], x1, x2, 0 }; // XXX g++ only!!
      a[an++] = (arc) { &c[i], x3, x4, 1 }; // XXX g++ only!!
    }
  }

  sort(a, a + an);

  double x1, x2, x3, x4, res = 0;
  for (int i = 0, b = 0; i < an; i++) {
    if (a[i].t == 0) {
      if (b == 0) {
        x1 = max(0.0, a[i].x1);
        x2 = max(0.0, a[i].x2);
        if (a[i].x1 + a[i].x2 > 0)
          res += area_bow(a[i].c->r, hypot(ya - yb, x1 - x2));
      }
      b++;
    } else {
      b--;
      if (b == 0) {
        x3 = min(X, a[i].x1);
        x4 = min(X, a[i].x2);
        res += (x3 + x4 - x1 - x2) * (yb - ya) / 2;
        if (a[i].x1 + a[i].x2 < X + X)
          res += area_bow(a[i].c->r, hypot(ya - yb, x3 - x4));
      }
    }
  }
  return res;
}

int main() {
  while (scanf("%lf %lf %d %d", &X, &Y, &n1, &n2), X > 0 || Y > 0 || n1 || n2) {
    for (int i = n2; i < n2 + n1; i++) {
      scanf("%lf %lf", &c[i].o.x, &c[i].o.y);
      c[i].r = 0.58;
    }
    for (int i = 0; i < n2; i++) {
      scanf("%lf %lf", &c[i].o.x, &c[i].o.y);
      c[i].r = 1.31;
    }
    n = n1 + n2;

    for (int i = 0; i < n; i++)
      if (c[i].o.x + c[i].r < 0 ||
          c[i].o.x - c[i].r > X ||
          c[i].o.y + c[i].r < 0 ||
          c[i].o.y - c[i].r > Y)
        c[i--] = c[--n];

    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++)
        if (contains(c[i], c[j]))
          c[j--] = c[--n];

    ysn = 0;
    ys[ysn++] = 0;
    ys[ysn++] = Y;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        point p1, p2;
        int num = ip_circle_circle(c[i], c[j], p1, p2);
        if (num >= 1) ys[ysn++] = p1.y;
        if (num >= 2) ys[ysn++] = p2.y;
      }
      ys[ysn++] = c[i].o.y;
      ys[ysn++] = c[i].o.y - c[i].r;
      ys[ysn++] = c[i].o.y + c[i].r;

      double y1, y2;
      if (ip_circle_x(c[i], 0, y1, y2)) {
        ys[ysn++] = y1;
        ys[ysn++] = y2;
      }
      if (ip_circle_x(c[i], X, y1, y2)) {
        ys[ysn++] = y1;
        ys[ysn++] = y2;
      }
    }
    sort(ys, ys + ysn);
    ysn = unique(ys, ys + ysn, deq) - ys;

    double ans = 0;
    for (int i = 0; i + 1 < ysn; i++)
      if (sign(0 - ys[i]) <= 0 && sign(ys[i + 1] - Y) <= 0) 
        ans += solve(ys[i], ys[i + 1]);
    printf("%.2lf\n", X * Y - ans + eps);
  }
  return 0;
}
