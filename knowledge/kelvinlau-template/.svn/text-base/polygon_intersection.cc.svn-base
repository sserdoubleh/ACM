#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

const int N = 1005;
const double eps = 1e-8;

inline int sign(double x) { 
  return x < -eps ? -1 : x > eps; 
}

struct point {
  double x, y;
  point(double x = 0, double y = 0) : x(x), y(y) {}

  bool operator<(const point &p) const { 
    return sign(x - p.x) * 2 + sign(y - p.y) < 0; 
  }
  bool operator==(const point &p) const { 
    return !sign(x - p.x) && !sign(y - p.y); 
  }
};

struct triangle {
  point p[3];
};

struct line {
  point p, q;
  line() {}
  line(point p, point q) : p(p), q(q) {}
};

double cross(point a, point b, point c) {
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

/* same side: 1; at least one of a, b touches l: 0; otherwise -1 */
int side(line m, point p, point q) {
  return sign(cross(m.p, m.q, p)) * sign(cross(m.p, m.q, q));
}

bool parallel(line u, line v) {
  return !sign((u.p.x - u.q.x) * (v.p.y - v.q.y) - (v.p.x - v.q.x) * (u.p.y - u.q.y));
}

/* u, v: line segment, inclusive */
bool intersected(line u, line v) {
  return !parallel(u, v) && side(u, v.p, v.q) <= 0 && side(v, u.p, u.q) <= 0;
}

/* intersection point */
point ip(line u, line v) {
  double n = (u.p.y - v.p.y) * (v.q.x - v.p.x) - (u.p.x - v.p.x) * (v.q.y - v.p.y);
  double d = (u.q.x - u.p.x) * (v.q.y - v.p.y) - (u.q.y - u.p.y) * (v.q.x - v.p.x);
  double r = n / d;
  return point(u.p.x + r * (u.q.x - u.p.x), u.p.y + r * (u.q.y - u.p.y));
}

bool inside_triangle(point a, point b, point c, point p) {
  if (cross(a, b, c) < 0) swap(b, c);
  return sign(cross(a, b, p)) > 0 && sign(cross(b, c, p)) > 0 && sign(cross(c, a, p)) > 0;
}

/* stores n - 2 splited triangles into T */
void triangulate(point p[], int n, triangle T[]) {
  list<point> P(p, p + n);
  list<point>::iterator a, b, c, q;
  triangle t;

  int m = 0;
  if (P.size() < 3) return;

  for (a = b = P.begin(), c = ++b, ++c; c != P.end(); a = b, c = ++b, ++c) if (sign(cross(*a, *b, *c)) > 0) {
    for (q = P.begin(); q != P.end(); q++) {
      if (q == a) { ++q; ++q; continue; }
      if (inside_triangle(*a, *b, *c, *q)) break;
    }
    if (q == P.end()) {
      t.p[0] = *a;
      t.p[1] = *b;
      t.p[2] = *c;
      T[m++] = t;
      
      P.erase(b);
      b = a;
      if (b != P.begin()) b--;
    }
  }
}

double area_polygon(point A[], int n) {
  double area = 0;
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    area += A[i].x * A[j].y - A[i].y * A[j].x;
  }
  return area / 2;
}

point o;

bool acmp(point a, point b) {
  return cross(o, a, b) > 0;
}

double triangles_intersection(triangle a, triangle b) {
  point p[15]; int n = 0;
  triangle T[2] = { a, b };

  for (int t = 0; t < 2; t++)
    for (int i = 0; i < 3; i++)
      if (inside_triangle(T[t].p[0], T[t].p[1], T[t].p[2], T[!t].p[i]))
        p[n++] = T[!t].p[i];
  
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      line l1(T[0].p[i], T[0].p[(i + 1) % 3]);
      line l2(T[1].p[j], T[1].p[(j + 1) % 3]);
      if (intersected(l1, l2))
        p[n++] = ip(l1, l2);
    }

  if (!n) return 0;

  sort(p, p + n);
  n = unique(p, p + n) - p;
  
  if (n >= 3) {
    o = p[0];
    sort(p + 1, p + n, acmp);
    return area_polygon(p, n);
  }
  return 0;
}

triangle t1[N], t2[N];

double polygon_intersection(point p[], int n, point q[], int m) {
  triangulate(p, n, t1);
  triangulate(q, m, t2);

  double area = 0;
  for (int i = 0; i < n - 2; i++)
    for (int j = 0; j < m - 2; j++)
      area += triangles_intersection(t1[i], t2[j]);

  return area;
}

int n, m;
point p[N], q[N];

int main() {
  double a1, a2;
  scanf("%d %d", &n, &m);
  for (int i = 0; i < n; i++) scanf("%lf %lf", &p[i].x, &p[i].y);
  for (int i = 0; i < m; i++) scanf("%lf %lf", &q[i].x, &q[i].y);
  if ((a1 = area_polygon(p, n)) < 0) reverse(p, p + n);
  if ((a2 = area_polygon(q, m)) < 0) reverse(q, q + m);
  printf("area of the union of two polygon is: %lf\n", fabs(a1) + fabs(a2) - polygon_intersection(p, n, q, m));
  return 0;
}
