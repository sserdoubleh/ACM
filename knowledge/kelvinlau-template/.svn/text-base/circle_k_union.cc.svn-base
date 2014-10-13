/* Spoj CIRUT
 * Given n circles, find the area of all k-union regions
 * NOTE: No duplicated circles are allowed!
 * O(n ^ 2 log(n)) */
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;

const double eps = 1e-8, inf = 1e+9, pi = acos(-1.0);

inline int sign(double x) { 
  return x < -eps ? -1 : x > eps; 
}

inline double sqr(double x) { 
  return x * x; 
}

struct point {
  double x, y;
};

struct circle {
  point o;
  double r;
};

struct event {
  double a;
  int t;
  point p;
  event() {}
  event(double a, int t, point p) : a(a), t(t), p(p) {}
  bool operator<(const event e) const { return a < e.a; }
};

inline int ip_circle_circle(
    const circle &c1, const circle &c2, point &p1, point &p2) {
  double mx = c2.o.x - c1.o.x, sx = c2.o.x + c1.o.x, mx2 = sqr(mx);
  double my = c2.o.y - c1.o.y, sy = c2.o.y + c1.o.y, my2 = sqr(my);
  double sq = mx2 + my2; 
  double d = -(sq - sqr(c1.r - c2.r)) * (sq - sqr(c1.r + c2.r));
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

inline double fix(double a, double b = 0) {
  a -= b;
  if (sign(a) < 0) a += 2 * pi;
  return a;
}

inline double angle(point a, point b) {
  return fix(atan2(b.y - a.y, b.x - a.x));
}

inline int contains(const circle &c1, const circle &c2) {
  return c1.r > c2.r &&
    sign(sqr(c1.o.x - c2.o.x) + sqr(c1.o.y - c2.o.y) - sqr(c1.r - c2.r)) <= 0;
}

inline double cross(point a, point b, point c) {
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

const int N = 1000 + 10;
int n, en;
circle cs[N];
event events[N + N];
point o;
double area[N];

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; i++)
    scanf("%lf %lf %lf", &cs[i].o.x, &cs[i].o.y, &cs[i].r);

  memset(area, 0, sizeof area);
  for (circle *a = cs; a < cs + n; a++) {
    int cover = 1;
    en = 0;
    for (circle *b = cs; b < cs + n; b++) if (a != b) {
      if (contains(*b, *a)) cover++;
      point p1, p2;
      if (ip_circle_circle(*a, *b, p1, p2) >= 2) {
        events[en++] = event(angle(a->o, p1), -sign(cross(a->o, b->o, p1)), p1);
        events[en++] = event(angle(a->o, p2), -sign(cross(a->o, b->o, p2)), p2);
        if ((events[en - 2].a < events[en - 1].a) ^
            (events[en - 2].t > events[en - 1].t))
          cover++;
      }
    }
    sort(events, events + en);
    events[en] = events[0];
    for (int i = 0; i < en; i++) {
      event *e1 = &events[i];
      event *e2 = &events[i + 1];
      cover += e1->t;
      double da = fix(e2->a, e1->a);
      area[cover] += cross(o, e1->p, e2->p) + sqr(a->r) * (da - sin(da));
    }
    if (!en) area[cover] += sqr(a->r) * pi * 2;
  }
  for (int i = 1; i < n; i++)
    area[i] -= area[i + 1];

  for (int i = 1; i <= n; i++)
    printf("[%d] = %.3lf\n", i, area[i] / 2);

  return 0;
}
