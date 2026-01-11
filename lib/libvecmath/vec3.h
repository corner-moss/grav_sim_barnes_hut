#ifndef VEC3_H
#define VEC3_H

typedef struct vec3 {
  double x1;
  double x2;
  double x3;
} vec3;

/* Should probably all be inlined */

double vec3_dot(vec3 a, vec3 b);
double vec3_abs2(vec3 a);
double vec3_abs(vec3 a);
double vec3_ang(vec3 a, vec3 b);
double vec3_l1(vec3 a);
double vec3_maxe(vec3 a);

vec3 vec3_sum(vec3 a, vec3 b);
vec3 vec3_diff(vec3 a, vec3 b);
vec3 vec3_sp(double a, vec3 b);
vec3 vec3_norm(vec3 a);
vec3 vec3_cross(vec3 a, vec3 b);
vec3 vec3_eabs(vec3 a);

#endif
