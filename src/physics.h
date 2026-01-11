#ifndef PHYISCS_H
#define PHYISCS_H

#include <libvecmath/vec3.h>

typedef struct mass {
  vec3 pos;
  vec3 vel;
  double m;
} mass;

/* Force between to masses as vector */
vec3 f_grav(mass m1, mass m2, double g, double norm);

/* Velocity of a circular orbit around center at distance dist */
double v_circular_orbit(mass center, double distance, double g);

/* Returns mass with stable circular orbit around center with distance distance
 * and normal to rotation plane normal.
 * Rotates clockwise around normal vector.
 * Only works if center mass is much larger than mass.
 * Is shifted by phase[rad] relative to x1-x3 plane. */
mass init_circular_orbit(mass center, double distance, vec3 normal,
                         double phase, double m, double g);

/* No idea what I am doing */
void init_galaxy(mass* m_arr, int n_m, double m_center, double m_other,
                 double g);
#endif /* PHYISCS_H */
