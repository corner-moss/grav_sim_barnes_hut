#include "sim_state.h"
#include "libvecmath/vec3.h"
#include "physics.h"

#include <stdlib.h>

void sim_state_init(sim_state* self, unsigned int n_m, double g, double norm,
                    double t_step) {
  self->n_m = n_m;
  self->m_array = calloc(self->n_m, sizeof(mass));
  /* Gravitational constant */
  self->g = g;
  /* Some constant for potential normalization */
  self->norm = norm;
  self->t_step = t_step;
}

void sim_state_cleanup(sim_state* self) { free(self->m_array); }

void sim_state_key_vals(sim_state* self) {
  vec3 c = {0};
  double m_t = 0;
  double l_x1 = 0;
  double h_x1 = 0;
  double l_x2 = 0;
  double h_x2 = 0;
  double l_x3 = 0;
  double h_x3 = 0;

  double m = 0;
  vec3 pos = {0};
  for (unsigned int i = 0; i < self->n_m; ++i) {
    m = self->m_array[i].m;
    pos = self->m_array[i].pos;

    m_t += m;
    c = vec3_sum(c, vec3_sp(m, pos));

    if (pos.x1 < l_x1) {
      l_x1 = pos.x1;
    } else if (pos.x1 > h_x1) {
      h_x1 = pos.x1;
    }

    if (pos.x2 < l_x2) {
      l_x2 = pos.x2;
    } else if (pos.x2 > h_x2) {
      h_x2 = pos.x2;
    }

    if (pos.x3 < l_x3) {
      l_x3 = pos.x3;
    } else if (pos.x3 > h_x3) {
      h_x3 = pos.x3;
    }
  }

  self->center_of_mass = vec3_sp(1 / m_t, c);
  self->m_tot = m_t;
  self->min_x1 = l_x1;
  self->max_x1 = h_x1;
  self->min_x2 = l_x2;
  self->max_x2 = h_x2;
  self->min_x3 = l_x3;
  self->max_x3 = h_x3;
}

void sim_state_update_positions(sim_state* self) {
  for (unsigned int i = 0; i < self->n_m; ++i) {
    self->m_array[i].pos = vec3_sum(
        self->m_array[i].pos, vec3_sp(self->t_step, self->m_array[i].vel));
  }
}
