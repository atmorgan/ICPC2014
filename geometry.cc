// From http://web.mit.edu/~ecprice/acm/acm07/
// Geometry - line intersection, circle intersection
#include <math.h>
#define EPS 1e-12
typedef struct { double x,y; } point;
typedef struct { point a, b; } line;
// Equality
int equals (double a, double b) { return (fabs(a-b) <= EPS); }
// Line intersection code
// Returns 0 if no intersection, 
//         -1 if infinite number of intersections, 1 otherwise
// Places any intersection point in the point pointer passed
int line_intersect (line l1, line l2, point *p) {
  double dx1, dx2, dy1, dy2, dy3, dx3;
  double m1,c1,m2,c2;

  dy1 = l1.a.y - l1.b.y;
  dx1 = l1.a.x - l1.b.x;

  dy2 = l2.a.y - l2.b.y;
  dx2 = l2.a.x - l2.b.x;

  if (equals(dy1 * dx2, dy2 * dx1)) {
    dy3 = l1.a.y - l2.a.y;
    dx3 = l1.a.x - l2.a.x;

    if (equals(dy1 * dx3, dy3 * dx1)) {
      // Infinite number of intersections
      return -1;
    }
    // No intersections
    return 0;
  }

  // Special case 1: l1 is vertical
  if (equals(dx1, 0.0)) {
    m2 = dy2 / dx2;
    c2 = l2.a.y - m2 * l2.a.x;
    p->x = l1.a.x;
    p->y = m2 * l1.a.x + c2;
    return 1;
  }

  // Special case 2: l2 is vertical 
  if (equals(dx2, 0.0)) {
    m1 = dy1 / dx1;
    c1 = l1.a.y - m1 * l1.a.x;
    p->x = l2.a.x;
    p->y = m1 * l2.a.x + c1;
    return 1;
  }

  // Calculate gradient, y intercept
  m1 = dy1 / dx1;
  c1 = l1.a.y - m1 * l1.a.x;

  m2 = dy2 / dx2;
  c2 = l2.a.y - m2 * l2.a.x;

  p->x = (c2 - c1) / (m1 - m2);
  p->y = m2 * p->x + c2;
  return 1;
}

void swap (double & x, double & y) {
  double t;

  t = x;
  x = y;
  y = t;
}

// Circle intersection code
// Returns -1 if infinite intersections, 0 if no intersections, 
// may return 1 or 2 otherwise, depending on number of intersections
// A circle is defined by a point (center) + a radius

int circle_intersect (point a, double ra, point b, double rb, point *p1, point *p2) {
  double aa,bb,cc,dd,tt;
  double bx, by;
  int s = 0;

  // Check for equal center
  if (equals(a.x, b.x) && equals(a.y, b.y)) {
    if (equals(ra, rb)) {
      return -1;
    }
    return 0;
  }
  // Shift everything assuming a = (0,0)
  bx = b.x - a.x;
  by = b.y - a.y;

  // Special case: by == 0
  if (equals(by, 0.0)) {
    s = 1;
    swap(bx,by);
  }

  tt = by * by + bx * bx + ra * ra - rb * rb;

  aa = 1 + (bx * bx) / (by * by);
  bb = 0 - (tt * bx) / by;
  cc = (1 / (4 * by * by)) * (tt * tt) - ra * ra;
  dd = bb * bb - 4 * aa * cc;
  if (equals(dd, 0)) {
    p1->x = -bb / (2 * aa);
    p1->y = (1 / (2 * by)) * (tt - 2 * bx * p1->x);
    if (s)
      swap(p1->x, p1->y);
    p1->x += a.x;
    p1->y += a.y;
    return 1;
  }

  p1->x = (1 / (2 * aa)) * (-bb + sqrt(dd));
  p1->y = (1 / (2 * by)) * (tt - 2 * bx * p1->x);

  p2->x = (1 / (2 * aa)) * (-bb - sqrt(dd));
  p2->y = (1 / (2 * by)) * (tt - 2 * bx * p2->x);

  if (s) {
    swap(p1->x, p1->y);
    swap(p2->x, p2->y);
  }

  p1->x += a.x;
  p1->y += a.y;

  p2->x += a.x;
  p2->y += a.y;

  return 2;
}
