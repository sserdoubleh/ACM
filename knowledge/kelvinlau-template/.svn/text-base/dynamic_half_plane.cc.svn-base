#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <map>
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
  double alpha;
  line() {}
  line(point a, point b) {
    p = a; q = b;
    alpha = atan2(q.y - p.y, q.x - p.x);
  }
  bool operator<(const line &l) const {
    return alpha < l.alpha;
  }
};

inline double cross(point a, point b, point c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

point ip(line u, line v) {
  double n = (u.p.y - v.p.y) * (v.q.x - v.p.x) - (u.p.x - v.p.x) * (v.q.y - v.p.y);
  double d = (u.q.x - u.p.x) * (v.q.y - v.p.y) - (u.q.y - u.p.y) * (v.q.x - v.p.x);
  double r = n / d;
  return point(u.p.x + r * (u.q.x - u.p.x), u.p.y + r * (u.q.y - u.p.y));
}

inline int side(line l, point p) {
  return sign(cross(l.p, l.q, p));
}

typedef map<line, point> polygon;

void forward(polygon &s, polygon::iterator &it) {
  if (++it == s.end()) it = s.begin();
}

void backward(polygon &s, polygon::iterator &it) {
  if (it == s.begin()) it = s.end(); --it;
}

int main() {
  int n;
  double w, h;
  point o;

  while (~scanf("%d %lf %lf", &n, &w, &h)) {
    point A(0, 0);
    point B(w, 0);
    point C(w, h);
    point D(0, h);

    polygon p;
    p[line(A, B)] = B;
    p[line(B, C)] = C;
    p[line(C, D)] = D;
    p[line(D, A)] = A;

    double area = 2 * w * h;

    for (int i = 0; i < n; i++) {
      point A, B;
      scanf("%lf %lf", &A.x, &A.y);
      scanf("%lf %lf", &B.x, &B.y);
      line l(A, B);

      if (!p.empty()) {
        polygon::iterator b = p.lower_bound(l);
        polygon::iterator a = b; backward(p, a);

        point prev = a->second;

        if (side(l, prev) < 0) {

          backward(p, b);
          backward(p, a);

          while (p.size() > 1 && side(l, a->second) <= 0) {
            area -= cross(o, a->second, b->second);
            p.erase(b);
            b = a;
            backward(p, a);
          }
          area -= cross(o, a->second, b->second);
          forward(p, a); forward(p, b);
          forward(p, a); forward(p, b);
          while (p.size() > 1 && side(l, a->second) <= 0) {
            area -= cross(o, prev, a->second);
            prev = a->second;
            p.erase(a);
            a = b;
            forward(p, b);
          }
          area -= cross(o, prev, a->second);
          if (p.size() > 1) {
            backward(p, a); backward(p, b);
            p[l] = ip(l, b->first);
            a->second = ip(a->first, l);
            area += cross(o, p[l], b->second);
            area += cross(o, a->second, p[l]);
            b = a; backward(p, a);
            area += cross(o, a->second, b->second);
          } else {
            p.clear();
            area = 0;
          }
        }
      }
      printf("%lf\n", area / 2);
    }
  }

  return 0;
}
