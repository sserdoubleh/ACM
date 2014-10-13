#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <list>
using namespace std;

const double eps = 1e-8, inf = 1e+9, pi = acos(-1.0);

inline int sign(double x) { 
  return x < -eps ? -1 : x > eps; 
}

inline double sqr(double x) { 
  return x * x; 
}

/* ---------- points ---------- */

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

void read(point *p) {
  scanf("%lf %lf", &p->x, &p->y);
}

void print(point p) {
  printf("%.3lf %.3lf\n", p.x, p.y);
}

point operator-(point a, point b) {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

double cross(point a, point b, point c) {
  return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

double dist(point a, point b) {
  return hypot(a.x - b.x, a.y - b.y);
}

/* mid point */
point mp(point a, point b) {
  return point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

/* next point */ 
point np(point a, double alpha, double d) {
  return point(a.x + d * cos(alpha), a.y + d * sin(alpha));
}

/* PA * PB */
double dot(point p, point a, point b) {
  return (a.x - p.x) * (b.x - p.x) + (a.y - p.y) * (b.y - p.y);
}

/* delta angle of ABC */
inline double delta_angle(point a, point b, point c) {
  return acos(dot(b, a, c) / (dist(a, b) * dist(b, c)));
}

/* if ABC a sharp triangle */
int sharp(point a, point b, point c) {
  return sign(dot(a, b, c)) > 0 && sign(dot(b, a, c)) > 0 && sign(dot(c, a, b)) > 0;
}

double fix(double a, double b = 0) {
  a -= b;
  if (sign(a) < 0) a += 2 * pi;
  if (sign(a - 2 * pi) >= 0) a -= 2 * pi;
  return a;
}

double angle(point a, point b) {
  return fix(atan2(b.y - a.y, b.x - a.x));
}

point rotate(point a, point b, double alpha) {
  double s, c, x, y;
  a.x -= b.x;
  a.y -= b.y;
  c = cos(alpha);
  s = sin(alpha);
  x = a.x * c - a.y * s;
  y = a.x * s + a.y * c;
  a.x = x + b.x;
  a.y = y + b.y;
  return a;
}

double area_heron(double a, double b, double c) {
  double s = (a + b + c) / 2.0;
  if (a > s || b > s || c > s) return -1;
  return sqrt(s * (s - a) * (s - b) * (s - c));
}

double area_triangle(point a, point b, point c) {
  return fabs(a.x * b.y + b.x * c.y + c.x * a.y - a.x * c.y - b.x * a.y - c.x * b.y) / 2;
}

double area_polygon(point a[], int n) {
  double area = 0;
  for (int j = 0, i = n - 1; j < n; i = j++)
    area += a[i].x * a[j].y - a[i].y * a[j].x;
  return area / 2;
}

point centroid(point a[], int n) {
  double area = 0;
  point c;
  
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    area += a[i].x * a[j].y - a[i].y * a[j].x;
    c.x += (a[i].x + a[j].x) * (a[i].x * a[j].y - a[i].y * a[j].x);
    c.y += (a[i].y + a[j].y) * (a[i].x * a[j].y - a[i].y * a[j].x);
  }
  area = fabs(area) / 2;
  c.x /= 6 * area;
  c.y /= 6 * area;
  return c;
}

/* ---------- convex hull ---------- */

point __o;

bool acmp(point a, point b) {
  int c = sign(cross(__o, a, b));
  return c > 0 || !c && dist(a, __o) < dist(b, __o);
}

/* find convex hull of p[n] in place
 * return # of points of resulting convex hull */
int find_convex(point p[], int n) {
  __o = *min_element(p, p + n);
  sort(p, p + n, acmp);
  
  int top = 0;
  point *stack = (point *)malloc(sizeof(point) * n); // XXX malloc!

  for (int i = 0; i < n; i++) {
    while (top >= 2 && sign(cross(stack[top - 2], stack[top - 1], p[i])) <= 0) top--;
    stack[top++] = p[i];
  }
  copy(stack, stack + top, p);
  free(stack);
  return top;
}

/* ---------- rotate calipers ---------- */

double shadow_length(double alpha, point a, point b) {
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  double c = cos(alpha);
  double s = sin(alpha);
  return fabs(dx * c + dy * s);
}

/* min area & min peri rectangle covering, using rotate calipers */
void rotate_calipers(point ps[], int n, double &area, double &peri)  {
  area = peri = inf;
  n = find_convex(ps, n);
  ps[n] = ps[0];

  point *q[4] = { NULL, NULL, NULL, NULL };
  for (int i = 0; i < n; i++) {
    point *p = &ps[i];
    if (!q[0] || q[0]->y > p->y || q[0]->y == p->y && q[0]->x > p->x) q[0] = p;
    if (!q[1] || q[1]->x < p->x || q[1]->x == p->x && q[1]->y > p->y) q[1] = p;
    if (!q[2] || q[2]->y < p->y || q[2]->y == p->y && q[2]->x < p->x) q[2] = p;
    if (!q[3] || q[3]->x > p->x || q[3]->x == p->x && q[3]->y < p->y) q[3] = p;
  }

  double alpha = 0;
  for (int k = 0; k < n + 5; k++) {
    int bi = -1;
    double gap_min = inf;
    for (int i = 0; i < 4; i++) {
      double gap = fix(angle(q[i][0], q[i][1]), alpha + i * pi / 2);
      if (gap < gap_min) {
        gap_min = gap;
        bi = i;
      }
    }
    if (++q[bi] == ps + n) q[bi] = ps + 0;
    alpha = fix(alpha + gap_min);

    double a = shadow_length(alpha + pi / 2, *q[0], *q[2]);
    double b = shadow_length(alpha, *q[1], *q[3]);
    area = min(area, a * b);
    peri = min(peri, a + a + b + b);
  }
}

/* ---------- lines ---------- */

struct line {
  point p, q;
  line() {}
  line(point p, point q) : p(p), q(q) {}
};

int parallel(line u, line v) {
  return !sign((u.p.x - u.q.x) * (v.p.y - v.q.y) - (v.p.x - v.q.x) * (u.p.y - u.q.y));
}

/* same side: 1; at least one of a, b touches l: 0; otherwise -1 */
int side(line m, point p, point q) {
  return sign(cross(m.p, m.q, p)) * sign(cross(m.p, m.q, q));
}

int on_line(line l, point p) {
  return !sign(cross(l.p, l.q, p));
}

/* u, v: line */
int coinside(line u, line v) {
  return on_line(u, v.p) && on_line(u, v.q);
}

/* u, v: line segment, inclusive */
int intersected(line u, line v) {
  return !parallel(u, v) && side(u, v.p, v.q) <= 0 && side(v, u.p, u.q) <= 0;
}

/* u, v: line segment, exclusive */
int intersected_exclusive(line u, line v) {
  return !parallel(u, v) && side(u, v.p, v.q) < 0 && side(v, u.p, u.q) < 0;
}

/* intersection point */
point ip(line u, line v) {
  double n = (u.p.y - v.p.y) * (v.q.x - v.p.x) - (u.p.x - v.p.x) * (v.q.y - v.p.y);
  double d = (u.q.x - u.p.x) * (v.q.y - v.p.y) - (u.q.y - u.p.y) * (v.q.x - v.p.x);
  double r = n / d;
  return point(u.p.x + r * (u.q.x - u.p.x), u.p.y + r * (u.q.y - u.p.y));
}

/* if P on the line segment l, inclusive */
int on_lineseg(line l, point P) {
  return on_line(l, P) && sign(dot(P, l.p, l.q)) <= 0;
}

/* if P on the line segment l, exclusive */
int on_lineseg_exclusive(line l, point P) {
  return on_line(l, P) && sign(dot(P, l.p, l.q)) < 0;
}

double dist_line_point(line l, point a) {
  return fabs(cross(l.p, l.q, a)) / dist(l.p, l.q);
}

double dist_lineseg_point(line l, point a) {
  if (on_lineseg(l, a)) return 0;
  if (on_line(l, a) || !sharp(l.p, a, l.q)) 
    return min(dist(l.p, a), dist(l.q, a));
  return dist_line_point(l, a);
}

/* u: line segment, ab: ray, p is the resulting intersection point */
int intersected_lineseg_ray(line u, line v, point &p) {
  if (parallel(u, v)) return 0;
  p = ip(u, v);
  return on_lineseg(u, p) && (on_lineseg(v, p) || on_lineseg(line(v.p, p), v.q));
}

/* if point a inside polygon p[n] */
int inside_polygon(point p[], int n, point a) {
  double sum = 0;
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    if (on_lineseg(line(p[i], p[j]), a)) return 0;
    double angle = acos(dot(a, p[i], p[j]) / dist(a, p[i]) / dist(a, p[j]));
    sum += sign(cross(a, p[i], p[j])) * angle;
  }
  return sign(sum);
}

/* if lineseg l strickly inside polygon p[n] */
int lineseg_inside_polygon(point p[], int n, line l) {
  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n;
    line l1(p[i], p[j]);
    if (on_lineseg_exclusive(l, p[i])) return 0;
    if (intersected_exclusive(l, l1)) return 0;
  }
  return inside_polygon(p, n, mp(l.p, l.q));
}

/* if lineseg l intersect convex polygon p[n] */
int intersect_convex_lineseg(point p[], int n, line l) {
  if (n < 3) return 0;
  
  point q[4]; int k = 0;
  q[k++] = l.p;
  q[k++] = l.q;
  for (int i = 0; i < n; i++) {
    if (on_lineseg(l, p[i])) {
      q[k++] = p[i];
    } else {
      int j = (i + 1) % n;
      line a(p[i], p[j]);
      point X = ip(a, l);
      if (on_lineseg(l, X) && on_lineseg(a, X)) q[k++] = X;
    }
  }
  
  sort(q, q + k);
  for (int i = 0; i + 1 < k; i++) {
    if (inside_polygon(p, n, mp(q[i], q[i + 1]))) return 1;
  }
  return 0;
}

line perpendicular(line l, point a) {
  return line(a, point(a.x + l.p.y - l.q.y, a.y + l.q.x - l.p.x));
}

point pedal(line l, point a) {
  return ip(l, perpendicular(l, a));
}

point mirror(line l, point a) {
  point p = pedal(l, a);
  return point(p.x * 2 - a.x, p.y * 2 - a.y);
}

point perpencenter(point a, point b, point c) {    
  line u = perpendicular(line(b, c), a);
  line v = perpendicular(line(a, c), b);  
  return ip(u, v);
}

/* Inscribed circle center */
point icc(point A, point B, point C) {
  double a = dist(B, C), b = dist(C, A), c = dist(A, B), 
         p = (a + b + c) / 2, 
         s = sqrt(p * (p - a) * (p - b) * (p - c));
  point cp;
  cp.x = (a * A.x + b * B.x + c * C.x) / (a + b + c); 
  cp.y = (a * A.y + b * B.y + c * C.y) / (a + b + c);
  return cp;
}

/* Perpendicular bisector */
line pb(point a, point b) {
  return perpendicular(line(a, b), mp(a, b));
}

/* circumcicle center */
point ccc(point A, point B, point C) {
  double a1 = B.x - A.x, b1 = B.y - A.y, c1 = (sqr(a1) + sqr(b1)) / 2;
  double a2 = C.x - A.x, b2 = C.y - A.y, c2 = (sqr(a2) + sqr(b2)) / 2;
  double d = a1 * b2 - a2 * b1;
  point cp;
  cp.x = A.x + (c1 * b2 - c2 * b1) / d; 
  cp.y = A.y + (a1 * c2 - a2 * c1) / d;
  return cp;
}

/* translate l with distance e and direction s */
line translate(line l, double e, int s) {
  double d = dist(l.p, l.q);
  double x = l.p.y - l.q.y;
  double y = l.q.x - l.p.x;
  x *= s * e / d;
  y *= s * e / d;
  l.p.x += x; l.p.y += y;
  l.q.x += x; l.q.y += y;
  return l;
}

/* area of the part of convex polygon p[n] on the positive side of l */
double cut_area(point *p, int n, line l) {
  int ai, bi;
  point ap, bp;
  ai = bi = -1;
  for (int i = 0; i < n; i++) {
    line v(p[i], p[i + 1]);
    if (parallel(v, l)) continue;
    point cp = ip(v, l);
    if (cp == p[i] || on_lineseg_exclusive(v, cp)) {
      if (ai == -1) {
        ai = i;
        ap = cp;
      } else {
        bi = i;
        bp = cp;
      }
    }
  }
  point *q = (point *)malloc(sizeof(point) * (n + 2)); // XXX malloc!
  int m = 0;
  for (int i = 0; i < n; i++) {
    if (sign(cross(l.p, l.q, p[i])) >= 0)
      q[m++] = p[i];
    if (i == ai)
      q[m++] = ap;
    if (i == bi)
      q[m++] = bp;
  }
  double res = area_polygon(q, m);
  free(q);
  return res;
}

/* ---------- polygon triangulation & intersection ---------- */

struct triangle {
  point p[3];
};

int inside_triangle_exclusive(point a, point b, point c, point p) {
  return sign(cross(a, b, p)) > 0 && sign(cross(b, c, p)) > 0 && sign(cross(c, a, p)) > 0;
}

int inside_triangle_inclusive(point a, point b, point c, point p) {
  return sign(cross(a, b, p)) >= 0 && sign(cross(b, c, p)) >= 0 && sign(cross(c, a, p)) >= 0;
}

/* stores n - 2 splited triangles into T */
void triangulate(point p[], int n, triangle T[]) {
  list<point> P(p, p + n);
  list<point>::iterator a, b, c, q;
  triangle t;

  int m = 0;
  if (P.size() < 3) return;

  for (a = b = P.begin(), c = ++b, ++c; c != P.end(); a = b, c = ++b, ++c) 
    if (sign(cross(*a, *b, *c)) > 0) {
      for (q = P.begin(); q != P.end(); q++) {
        if (q == a) { ++q; ++q; continue; }
        if (inside_triangle_inclusive(*a, *b, *c, *q)) break;
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

point o1;

bool acmp1(point a, point b) {
  return cross(o1, a, b) > 0;
}

double triangles_intersection(triangle a, triangle b) {
  point p[15]; 
  int n = 0;
  triangle T[2] = { a, b };

  for (int t = 0; t < 2; t++)
    for (int i = 0; i < 3; i++)
      if (inside_triangle_exclusive(T[t].p[0], T[t].p[1], T[t].p[2], T[!t].p[i]))
        p[n++] = T[!t].p[i];
  
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      line l1(T[0].p[i], T[0].p[(i + 1) % 3]);
      line l2(T[1].p[j], T[1].p[(j + 1) % 3]);
      if (intersected(l1, l2))
        p[n++] = ip(l1, l2);
    }

  if (!n) return 0.0;

  sort(p, p + n);
  n = unique(p, p + n) - p;

  if (n < 3)
    return 0.0;
  
  o1 = p[0];
  sort(p + 1, p + n, acmp1);
  return area_polygon(p, n);
}

double polygon_intersection(point p[], int n, point q[], int m) {
  triangle *t1 = (triangle *)malloc(sizeof(triangle) * n); // XXX malloc!
  triangle *t2 = (triangle *)malloc(sizeof(triangle) * n); // XXX malloc!

  triangulate(p, n, t1);
  triangulate(q, m, t2);

  double area = 0;
  for (int i = 0; i < n - 2; i++)
    for (int j = 0; j < m - 2; j++)
      area += triangles_intersection(t1[i], t2[j]);

  free(t1);
  free(t2);

  return area;
}

/* ---------- intersection points convex hull ---------- */

bool lcmp(line u, line v) {
  int c = sign((u.p.x - u.q.x) * (v.p.y - v.q.y) - (v.p.x - v.q.x) * (u.p.y - u.q.y));
  return c < 0 || !c && sign(cross(u.p, u.q, v.p)) < 0;
}

/* XXX sizeof(p) MUST be as large as n * 2
 * return # of points of resulting convex hull */
int ip_convex(line l[], int n, point p[]) {

  for (int i = 0; i < n; i++)
    if (l[i].q < l[i].p) swap(l[i].p, l[i].q);

  sort(l, l + n, lcmp);

  int n1 = 0;
  for (int i = 0, j = 0; i < n; i = j) {
    while (j < n && parallel(l[i], l[j])) j++;
    if (j - i == 1) {
      l[n1++] = l[i];
    } else {
      l[n1++] = l[i];
      l[n1++] = l[j - 1];
    }
  }
  n = n1;

  l[n + 0] = l[0]; 
  l[n + 1] = l[1];

  int m = 0;
  for (int i = 0, j = 0; i < n; i++) {
    while (j < n + 2 && parallel(l[i], l[j])) j++;
    for (int k = j; k < n + 2 && parallel(l[j], l[k]); k++)
      p[m++] = ip(l[i], l[k]);
  }

  return find_convex(p, m);
}

/* ---------- circles ---------- */

struct circle {
  point o;
  double r;
  circle(point o = point(), double r = 1) : o(o), r(r) {}
  circle(double x, double y, double r = 1) : o(x, y), r(r) {}
};

int intersected_circle_line(circle c, line l) {
  return sign(dist_line_point(l, c.o) - c.r) < 0;
}

int ip_circle_line(circle c, line l, point &p1, point &p2) {
  point a = l.p, b = l.q;
  double dx = b.x - a.x, dy = b.y - a.y;
  double sdr = sqr(dx) + sqr(dy), dr = sqrt(sdr);
  double d, disc, x, y;
  
  a.x -= c.o.x; a.y -= c.o.y;
  b.x -= c.o.x; b.y -= c.o.y;
  d = a.x * b.y - b.x * a.y;
  disc = sqr(c.r) * sdr - sqr(d);
  
  if (disc < -eps) return 0;
  if (disc < +eps) disc = 0; else disc = sqrt(disc);
  x = disc * dx * (dy > 0 ? 1 : -1);
  y = disc * fabs(dy);
  p1.x = (+d * dy + x) / sdr + c.o.x;
  p2.x = (+d * dy - x) / sdr + c.o.x;
  p1.y = (-d * dx + y) / sdr + c.o.y;
  p2.y = (-d * dx - y) / sdr + c.o.y;
  return disc > eps ? 2 : 1;
}

int ip_circle_circle(const circle &c1, const circle &c2, point &p1, point &p2) {
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

double circle_circle_intersection_area(circle A, circle B) {
  double d, dA, dB, tx, ty;

  d = hypot(B.o.x - A.o.x, B.o.y - A.o.y);

  if ((d < eps) || (d + A.r <= B.r) || (d + B.r <= A.r))
    return sqr((B.r < A.r) ? B.r : A.r) * pi;
  if (d >= A.r + B.r)
    return 0;

  dA = tx = (sqr(d) + sqr(A.r) - sqr(B.r)) / d / 2;
  ty = sqrt(sqr(A.r) - sqr(tx));
  dB = d - dA;

  return sqr(A.r) * acos(dA / A.r) - dA * sqrt(sqr(A.r) - sqr(dA)) +
         sqr(B.r) * acos(dB / B.r) - dB * sqrt(sqr(B.r) - sqr(dB));
}

/* return 2 points of tangency of c and p */
void circle_tangents(circle c, point p, point &a, point &b) {
  double d = sqr(c.o.x - p.x) + sqr(c.o.y - p.y);
  double para = sqr(c.r) / d;
  double perp = c.r * sqrt(d - sqr(c.r)) / d;
  a.x = c.o.x + (p.x - c.o.x) * para - (p.y - c.o.y) * perp;
  a.y = c.o.y + (p.y - c.o.y) * para + (p.x - c.o.x) * perp;
  b.x = c.o.x + (p.x - c.o.x) * para + (p.y - c.o.y) * perp;
  b.y = c.o.y + (p.y - c.o.y) * para - (p.x - c.o.x) * perp;
}

/* 0: oncircle; 1: inside circle; -1: outside circle*/
int on_circle(circle c, point a) {
  return sign(c.r - dist(a, c.o));
}

/* minimum circle that covers 2 points */
circle cc2(point a, point b) {
  return circle(mp(a, b), dist(a, b) / 2);
}

/* minimum circle that covers 3 points */
circle cc3(point p, point q, point r) {
  circle c;

  if (on_circle(c = cc2(p, q), r) >= 0) return c;
  if (on_circle(c = cc2(p, r), q) >= 0) return c;
  if (on_circle(c = cc2(q, r), p) >= 0) return c;

  c.o = ccc(p, q, r);
  c.r = dist(c.o, p);
  return c;
}

/* minimum circle that covers n points */
circle min_circle_cover(point p[], int n) {
  if (n == 1) return circle(p[0], 0);
  if (n == 2) return cc2(p[0], p[1]);

  random_shuffle(p, p + n);
  point *ps[4] = { &p[0], &p[1], &p[2], &p[3] };
  circle c = cc3(*ps[0], *ps[1], *ps[2]);

  while (1) {
    point *b = p;
    for (int i = 1; i < n; i++)
      if (dist(p[i], c.o) > dist(*b, c.o)) b = &p[i];
    if (on_circle(c, *b) >= 0) return c;

    ps[3] = b;
    for (int i = 0; i < 3; i++) {
      swap(ps[i], ps[3]);
      if (on_circle(c = cc3(*ps[0], *ps[1], *ps[2]), *ps[3]) >= 0) break;
    }
  }
}

int main() {
}
