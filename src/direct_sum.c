#include "direct_sum.h"
#include "sim_state.h"
#include "physics.h"

#include <libvecmath/vec3.h>
#include <math.h>

void direct_sum(struct sim_state* state) {
  double c = 0;
  vec3 diff = {0};
  for (unsigned int i = 0; i < state->n_m - 1; ++i) {
    for (unsigned int j = i + 1; j < state->n_m; ++j) {
      diff = vec3_diff(state->m_array[j].pos, state->m_array[i].pos);
      c = state->g / pow(vec3_abs2(diff), 0.75);
      state->m_array[i].vel = vec3_sum(state->m_array[i].vel,
                                       vec3_sp(c * state->m_array[j].m, diff));
      state->m_array[i].vel = vec3_sum(state->m_array[i].vel,
                                       vec3_sp(-c * state->m_array[j].m, diff));
    }
  }
}
