#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
using namespace std;

const double eps = 1e-6;

inline int sign(double x) {
  return x < -eps ? -1 : x > eps;
}

inline double sqr(double x) {
  return x * x;
}

inline double frand() {
  return rand() / (RAND_MAX + 1.0);
}

/* ---------- points & vectors ---------- */

typedef struct point {
  double x, y, z;
} vector;

void read(point *p) {
  scanf("%lf %lf %lf\n", &p->x, &p->y, &p->z);
}

void print(point p) {
  printf("%lf %lf %lf\n", p.x, p.y, p.z);
}

inline vector operator+(point a, point b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

inline vector operator-(point a, point b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}

inline vector operator*(double t, vector a) {
  a.x *= t;
  a.y *= t;
  a.z *= t;
  return a;
}

inline vector operator*(vector a, double t) {
  a.x *= t;
  a.y *= t;
  a.z *= t;
  return a;
}

inline vector operator/(vector a, double t) {
  a.x /= t;
  a.y /= t;
  a.z /= t;
  return a;
}

inline vector operator*(vector a, vector b) {
  vector c;
  c.x = a.y * b.z - a.z * b.y;
  c.y = a.z * b.x - a.x * b.z;
  c.z = a.x * b.y - a.y * b.x;
  return c;
}

inline double operator^(vector a, vector b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double len(vector a) {
  return sqrt(a ^ a);
}

inline double len2(vector a) {
  return a ^ a;
}

inline int zero(vector a) {
  return !sign(a.x) && !sign(a.y) && !sign(a.z);
}

/* ---------- lines, line segments & planes ---------- */

/* returns a vector that perps to u */
vector perp_vector(vector u) {
  vector v, n;
  while (1) {
    v.x = frand();
    v.y = frand();
    v.z = frand();
    if (!zero(n = u * v)) return n;
  }
}

int on_seg(point p, point q, point A) {
  return zero((A - p) * (A - q)) && sign((p - A) ^ (q - A)) <= 0;
}

/* relation of C & D based on AB
 * same side:     +1; 
 * opposite side: -1; 
 * otherwise:      0 */
inline int side(point A, point B, point C, point D) {
  return sign(((A - B) * (C - A)) ^ ((A - B) * (D - A)));
}

/* intersection point of plane(norm, A) and lineseg pq, X is the result */
int ip_plane_seg(vector norm, point A, point p, point q, point &X) {
  double lhs = norm ^ (q - p);
  double rhs = norm ^ (A - p);
  double t = rhs / lhs;
  if (sign(t) >= 0 && sign(t - 1) <= 0) {
    X = p + t * (q - p);
    return 1;
  }
  return 0;
}

/* check if 2 linesegs AB & CD touched with each other */
int touched_segs(point A, point B, point C, point D) {
  if (zero((A - B) * (C - D)))
    return on_seg(A, B, C) || on_seg(A, B, D) || on_seg(C, D, A) || on_seg(C, D, B);
  else
    return side(C, D, A, B) <= 0 && side(A, B, C, D) <= 0;
}

point pedal(point a, point b, point p) {
  double t = ((b - a) ^ (p - a)) / ((b - a) ^ (b - a));
  return a + t * (b - a);
}

point closest_point_seg(point a, point b, point p) {
  double t = ((b - a) ^ (p - a)) / ((b - a) ^ (b - a));
  return a + max(0.0, min(t, 1.0)) * (b - a);
}

int on_plane(vector n, point a, point p) {
  return !sign(n ^ (p - a));
}

double dist_plane_point(vector n, point a, point p) {
  return fabs(n ^ (p - a)) / len(n);
}

point closest_point_plane(vector n, point a, point p) {
  return p + ((n ^ (a - p)) / (n ^ n)) * n;
}

/* mapping from 3D point to 2D point */
point to_plane(point a, point b, point c, point p) {
  vector norm, ydir, xdir;
  point res;

  norm = (b - a) * (c - a);
  xdir = b - a;
  xdir = xdir / len(xdir);
  ydir = norm * xdir;
  ydir = ydir / len(ydir);
  res.x = (p - a) ^ xdir;
  res.y = (p - a) ^ ydir;
  res.z = 0;
  return res;
}

/* given two lines in 3D space, find distance of closest approach */
double dist_line_line(point a, point b, point c, point d) {
  vector v = (b - a) * (d - c);
  if (zero(v)) {
    if (zero((b - a) * (c - a))) return 0;
    return len((c - a) * (d - a)) / len(c - d);
  }
  return fabs((a - c) ^ v) / len(v);
}

/* this is the same as dist_line_line, but it also returns
   the points of closest approach */
double closest_approach(point a, point b, point c, point d, point &p, point &q) {
  double s = (d - c) ^ (b - a), t = (a - c) ^ (d - c);
  double num, den, tmp;

  den = len2(b - a) * len2(d - c) - s * s;
  num = t * s - len2(d - c) * ((a - c) ^ (b - a));
  if (!sign(den)) {
    p = a;
    q = c + (d - c) * t / len2(d - c);
    if (!sign(s)) q = a;
  } else {
    tmp = num / den;
    p = a + (b - a) * tmp;
    q = c + (d - c) * (t + s * tmp) / len2(d - c);
  }
  return len(p - q);
}

/* ---------- balls (spheres) ---------- */

int ip_ball_line(point o, double r, point p, point q, point &a, point &b) {
  vector v;
  point d = pedal(p, q, o);
  if (len2(o - d) > r * r) return 0;
  v = sqrt((r * r - len2(o - d)) / len2(p - q)) * (p - q);
  a = d + v;
  b = d - v;
  return 1;
}

/* Given the latitude and longitude of two points in degrees, 
 * calculates the distance over the sphere between them. 
 * Latitude is given in the range [-pi/2, pi/2] degrees, 
 * Longitude is given in the range [-pi,pi] degrees.
 */
double greatcircle(double lat1, double long1, double lat2, double long2) {
  return acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long2 - long1));
}

/* Solves the determinant of a n*n matrix recursively */
double det(double m[4][4], int n) {
  double s[4][4], res = 0, x;
  int i, j, skip, ssize;

  if (n == 2)
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
  for (skip = 0; skip < n; skip++) {
    for (i = 1; i < n; i++)
      for (j = 0, ssize = 0; j < n; j++) {
        if (j == skip)
          continue;
        s[i - 1][ssize++] = m[i][j];
      }
    x = det(s, n - 1);
    if (skip % 2)
      res -= m[0][skip] * x;
    else
      res += m[0][skip] * x;
  }
  return res;
}

/* Given 4 points:
 * Returns 0 if the points are coplanar 
 * Returns 1 if the points are not coplanar with:
 *   o = center of sphere
 *   r = radius of sphere
 */
int make_sphere(point p[4], point &o, double &r) {
  double m[4][5], s[4][4], sol[5];
  int ssize, skip, i, j;

  for (i = 0; i < 4; i++) {
    s[i][0] = p[i].x;
    s[i][1] = p[i].y;
    s[i][2] = p[i].z;
    s[i][3] = 1;
  }
  if (!sign(det(s, 4)))
    return 0;

  for (i = 0; i < 4; i++) {
    m[i][0] = sqr(m[i][1] = p[i].x)
        + sqr(m[i][2] = p[i].y)
        + sqr(m[i][3] = p[i].z);
    m[i][4] = 1;
  }

  for (skip = 0; skip < 5; skip++) {
    for (i = 0; i < 4; i++)
      for (j = 0, ssize = 0; j < 5; j++) if (j != skip) {
        s[i][ssize++] = m[i][j];
      }
    sol[skip] = det(s, 4);
  }

  for (i = 1; i < 5; i++)
    sol[i] /= (sol[0] * ((i % 2) ? 1 : -1));

  for (i = 1; i < 4; i++)
    sol[4] += sqr(sol[i] /= 2);

  o.x = sol[1];
  o.y = sol[2];
  o.z = sol[3];
  r = sqrt(sol[4]);
  return 1;
}

/* ---------- polygons ---------- */

/* check if point A inside polygon p[n] */
int inside_polygon(point *p, int n, vector norm, point A) {
  if (sign(norm ^ (A - p[0]))) return 0;

  for (int i = 0; i < n; i++)
    if (on_seg(p[i], p[i + 1], A)) return 1;

  double sum = 0;
  for (int i = 0; i < n; i++) {
    vector a = p[i] - A;
    vector b = p[i + 1] - A;
    sum += sign(norm ^ (a * b)) * acos((a ^ b) / (len(a) * len(b)));
  }
  return sign(sum);
}

/* check if lineseg AB touches polygon p[n] with normal vector norm */
int intersected_polygon_seg(point *p, int n, vector norm, point A, point B) {
  if (!sign((A - B) ^ norm)) {
    if (sign(norm ^ (A - p[0]))) return 0;
    if (inside_polygon(p, n, norm, A) || inside_polygon(p, n, norm, B)) return 1;
    for (int i = 0; i < n; i++)
      if (touched_segs(A, B, p[i], p[i + 1])) return 1;
    return 0;
  }
  point X;
  if (ip_plane_seg(norm, p[0], A, B, X))
    return inside_polygon(p, n, norm, X);
  return 0;
}

/* normal vector of polygon p[n] */
vector normal(point *p, int n) {
  vector b, norm;
  p[n] = p[0]; p[n + 1] = p[1];
  for (int i = 0; i < n; i++)
    if (!zero(norm = (p[i + 1] - p[i + 2]) * (p[i] - p[i + 1])))
      return norm;
  return perp_vector(p[0] - p[1]);
}

/* check if 2 polygons p[n] & q[m] touched with each other */
int touched_polygons(point *p, int n, point *q, int m) {
  vector norm;
  
  norm = normal(q, m);
  for (int i = 0; i < n; i++)
    if (intersected_polygon_seg(q, m, norm, p[i], p[i + 1])) return 1;

  norm = normal(p, n);
  for (int i = 0; i < m; i++)
    if (intersected_polygon_seg(p, n, norm, q[i], q[i + 1])) return 1;

  return 0;
}
