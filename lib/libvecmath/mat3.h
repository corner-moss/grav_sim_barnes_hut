#ifndef MAT3_H
#define MAT3_H

#include "vec3.h"

typedef struct mat3 {
  double x11;
  double x12;
  double x13;
  double x21;
  double x22;
  double x23;
  double x31;
  double x32;
  double x33;
} mat3;

/* mat3 -> double */
double mat3_trace(mat3 m);
double mat3_det(mat3 m);

/* Matrix operations */
mat3 mat3_sp(double a, mat3 m);
mat3 mat3_sum(mat3 m, mat3 n);
mat3 mat3_diff(mat3 m, mat3 n);

mat3 mat3_transpose(mat3 m);
mat3 mat3_matmul(mat3 m, mat3 n);

/* Rotation */
/* Returns rotation matricies for rad around the main axis */
mat3 mat3_rotation_x1(double rad);
mat3 mat3_rotation_x2(double rad);
mat3 mat3_rotation_x3(double rad);
/* Returns rotation of x by rad around normal */
vec3 mat3_rotation_vn(vec3 x, vec3 normal, double rad);

/* Matrix vector product */
vec3 mat3_apply(mat3 m, vec3 v);

#endif // MAT3_H
