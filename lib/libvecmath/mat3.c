#include "mat3.h"
#include "vec3.h"

#include <math.h>

double mat3_trace(mat3 m) { return m.x11 + m.x22 + m.x33; }

double mat3_det(mat3 m) {
  double det = m.x11 * m.x22 * m.x33;
  det += m.x12 * m.x23 * m.x31;
  det += m.x13 * m.x21 * m.x32;
  det -= m.x31 * m.x22 * m.x13;
  det -= m.x21 * m.x12 * m.x33;
  det -= m.x11 * m.x32 * m.x23;

  return det;
}

mat3 mat3_sum(mat3 m, mat3 n) {
  m.x11 += n.x11;
  m.x12 += n.x12;
  m.x13 += n.x13;
  m.x21 += n.x21;
  m.x22 += n.x22;
  m.x23 += n.x23;
  m.x31 += n.x31;
  m.x32 += n.x32;
  m.x33 += n.x33;

  return m;
}

mat3 mat3_diff(mat3 m, mat3 n) {
  m.x11 -= n.x11;
  m.x12 -= n.x12;
  m.x13 -= n.x13;
  m.x21 -= n.x21;
  m.x22 -= n.x22;
  m.x23 -= n.x23;
  m.x31 -= n.x31;
  m.x32 -= n.x32;
  m.x33 -= n.x33;

  return m;
}

mat3 mat3_transpose(mat3 m) {
  mat3 ret = m;
  ret.x11 = m.x33;
  ret.x12 = m.x21;
  ret.x21 = m.x12;
  ret.x13 = m.x31;
  ret.x31 = m.x13;
  ret.x23 = m.x32;
  ret.x32 = m.x23;

  return ret;
}

mat3 mat3_matmul(mat3 m, mat3 n) {
  vec3 m1 = {.x1 = m.x11, .x2 = m.x12, .x3 = m.x13};
  vec3 m2 = {.x1 = m.x21, .x2 = m.x22, .x3 = m.x23};
  vec3 m3 = {.x1 = m.x31, .x2 = m.x32, .x3 = m.x33};
  vec3 n1 = {.x1 = n.x11, .x2 = n.x21, .x3 = n.x31};
  vec3 n2 = {.x1 = n.x12, .x2 = n.x22, .x3 = n.x32};
  vec3 n3 = {.x1 = n.x13, .x2 = n.x23, .x3 = n.x33};

  mat3 ret = {0};
  ret.x11 = vec3_dot(m1, n1);
  ret.x12 = vec3_dot(m1, n2);
  ret.x13 = vec3_dot(m1, n3);
  ret.x21 = vec3_dot(m2, n1);
  ret.x22 = vec3_dot(m2, n2);
  ret.x23 = vec3_dot(m2, n3);
  ret.x31 = vec3_dot(m3, n1);
  ret.x32 = vec3_dot(m3, n2);
  ret.x33 = vec3_dot(m3, n3);

  return ret;
}

mat3 mat3_rotation_x1(double rad) {
  mat3 ret = {0};
  ret.x22 = cos(rad);
  ret.x23 = sin(rad);
  ret.x32 = -sin(rad);
  ret.x33 = ret.x11;
  ret.x11 = 1;

  return ret;
}

mat3 mat3_rotation_x2(double rad) {
  mat3 ret = {0};
  ret.x11 = cos(rad);
  ret.x13 = sin(rad);
  ret.x31 = -sin(rad);
  ret.x33 = ret.x11;
  ret.x22 = 1;

  return ret;
}

mat3 mat3_rotation_x3(double rad) {
  mat3 ret = {0};
  ret.x11 = cos(rad);
  ret.x12 = -sin(rad);
  ret.x21 = sin(rad);
  ret.x22 = ret.x11;
  ret.x33 = 1;

  return ret;
}

vec3 mat3_rotation_vn(vec3 x, vec3 normal, double rad) {
  normal = vec3_norm(normal);
  vec3 a = vec3_sp(vec3_dot(normal, x), normal);
  vec3 b = vec3_sp(cos(rad), vec3_cross(vec3_cross(normal, x), normal));
  vec3 c = vec3_sp(sin(rad), vec3_cross(normal, x));

  return vec3_sum(vec3_sum(a, b), c);
}

vec3 mat3_apply(mat3 m, vec3 v) {
  vec3 ret = {0};
  ret.x1 = m.x11 * v.x1 + m.x12 * v.x2 + m.x13 * v.x3;
  ret.x2 = m.x21 * v.x1 + m.x22 * v.x2 + m.x23 * v.x3;
  ret.x3 = m.x31 * v.x1 + m.x32 * v.x2 + m.x33 * v.x3;

  return ret;
}
