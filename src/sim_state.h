#ifndef SIM_STATE_H
#define SIM_STATE_H

#include "physics.h"

#include <libvecmath/vec3.h>

typedef struct sim_state {
  /* Mass array */
  unsigned int n_m;
  mass* m_array;

  /* physics paramters */
  double g;
  double norm;
  double t_step;

  /* Attributes */
  vec3 center_of_mass;
  double m_tot;
  double min_x1;
  double max_x1;
  double min_x2;
  double max_x2;
  double min_x3;
  double max_x3;
} sim_state;

/* Inits mass array of n_m masses initialized to zero */
void sim_state_init(sim_state* self, unsigned int n_m, double g, double norm,
                    double t_step);
void sim_state_cleanup(sim_state* self);

/* Find extremal position values and center off mass and total mass and store
 * them in sim_state fields */
void sim_state_key_vals(sim_state* self);

#endif // SIM_STATE_H
