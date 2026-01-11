#include "physics.h"

#include <libvecmath/mat3.h>
#include <libvecmath/vec3.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

vec3 f_grav(mass m1, mass m2, double g, double norm) {
  double p = g * m1.m * m2.m;
  ;
  vec3 diff = vec3_diff(m2.pos, m1.pos);
  double denom = vec3_abs(diff);

  return vec3_sp(p / (pow(denom, 3) + norm), diff);
}

double v_circular_orbit(mass center, double dist, double g) {
  return sqrt(g * center.m / dist);
}

mass init_circular_orbit(mass center, double distance, vec3 normal,
                         double phase, double m, double g) {
  double v = v_circular_orbit(center, distance, g);
  normal = vec3_norm(normal);

  mass ret;
  ret.m = m;

  // ret.pos = (vec3){.x1 = 0, dista}
  ret.pos = (vec3){.x1 = distance, .x2 = 0.0, .x3 = 0.0};
  ret.vel = vec3_cross(normal, (vec3){.x1 = 1.0, .x2 = 0.0, .x3 = 0.0});
  ret.vel = vec3_sp(v, ret.vel);

  ret.pos = mat3_rotation_vn(ret.pos, normal, phase);
  ret.vel = mat3_rotation_vn(ret.vel, normal, phase);

  ret.pos = vec3_sum(ret.pos, center.pos);

  return ret;
}

void init_galaxy(mass* m_arr, int n_m, double m_center, double m_other,
                 double g) {
  m_arr[0].m = m_center;

  srand(time(NULL));
  double phase = 0;
  double distance = 0;

  for (int i = 1; i < n_m; ++i) {
    phase = 2 * M_PI * rand() / RAND_MAX;
    distance = 1.0 * rand() / RAND_MAX;

    m_arr[i] = init_circular_orbit(m_arr[0], distance, (vec3){0, 0, 1}, phase,
                                   m_other, g);
  }
}
