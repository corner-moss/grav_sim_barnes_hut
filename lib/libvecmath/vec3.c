#include "vec3.h"
#include <math.h>

double vec3_dot(vec3 a, vec3 b) {
  return a.x1 * b.x1 + a.x2 * b.x2 + a.x3 * b.x3;
}

double vec3_abs2(vec3 a) { return a.x1 * a.x1 + a.x2 * a.x2 + a.x3 * a.x3; }

double vec3_abs(vec3 a) { return sqrt(vec3_abs2(a)); }

vec3 vec3_eabs(vec3 a) {
  a.x1 = fabs(a.x1);
  a.x2 = fabs(a.x2);
  a.x3 = fabs(a.x3);
  return a;
}

double vec3_maxe(vec3 a) {
  double max = 0;
  max = a.x1 > a.x2 ? a.x1 : a.x2;
  max = a.x3 > max ? a.x3 : max;
  return max;
}

double vec3_l1(vec3 a) { return vec3_maxe(vec3_eabs(a)); }

double vec3_ang(vec3 a, vec3 b) {
  double sp = vec3_dot(a, b);
  double norm = vec3_abs(a) * vec3_abs(b);
  return acos(sp / norm);
}

vec3 vec3_sum(vec3 a, vec3 b) {
  vec3 s = {0};
  s.x1 = a.x1 + b.x1;
  s.x2 = a.x2 + b.x2;
  s.x3 = a.x3 + b.x3;
  return s;
}

vec3 vec3_diff(vec3 a, vec3 b) {
  vec3 s = {0};
  s.x1 = a.x1 - b.x1;
  s.x2 = a.x2 - b.x2;
  s.x3 = a.x3 - b.x3;
  return s;
}

vec3 vec3_sp(double a, vec3 v) {
  v.x1 = a * v.x1;
  v.x2 = a * v.x2;
  v.x3 = a * v.x3;

  return v;
}

vec3 vec3_norm(vec3 a) {
  double inv = 1 / vec3_abs(a);
  return vec3_sp(inv, a);
}

vec3 vec3_cross(vec3 a, vec3 b) {
  vec3 ret = {0};
  ret.x1 = a.x2 * b.x3 - a.x3 * b.x2;
  ret.x2 = a.x3 * b.x1 - a.x1 * b.x3;
  ret.x3 = a.x1 * b.x2 - a.x2 * b.x1;

  return ret;
}
