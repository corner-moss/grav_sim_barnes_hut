#include "octree.h"
#include "sim_state.h"

#include <libvecmath/vec3.h>
#include <stdlib.h>
#include <math.h>

struct octree octree_init(unsigned int stack_size, double threshhold,
                          double tolerance) {
  struct octree self = {0};
  self.stack = calloc(stack_size, sizeof(struct octree_node));
  self.stack_size = stack_size;
  self.threshhold = threshhold;
  self.tolerance = tolerance;

  return self;
}

void octree_set_root(struct octree* self, struct sim_state* state) {
  vec3 max =
      (vec3){.x1 = state->max_x1, .x2 = state->max_x2, .x3 = state->max_x3};
  vec3 min =
      (vec3){.x1 = state->min_x1, .x2 = state->min_x2, .x3 = state->min_x3};

  vec3 center = vec3_sp(0.5, vec3_sum(max, min));
  double max_max = vec3_l1(vec3_diff(max, center));
  double min_max = vec3_l1(vec3_diff(min, center));

  self->stack[0].center = center;
  self->stack[0].edge_length = min_max + max_max;
}

void octree_prepare(struct octree* self, struct sim_state* state) {
  for (unsigned int i = 0; i < self->number_nodes; ++i) {
    self->stack[i] = (struct octree_node){0};
  }
  self->number_nodes = 1;
  octree_set_root(self, state);
}

unsigned int octree_is_leaf(struct octree* self, unsigned int node) {
  unsigned int ret = 0;
  for (unsigned int i = 0; i < 8; ++i) {
    ret += self->stack[node].children[i];
  }
  return node && !ret;
}

unsigned int octree_octant(const vec3 center, const vec3 pos) {
  unsigned int octant = 0;
  if (pos.x1 - center.x1 > 0) {
    octant = 1;
  }
  octant <<= 1;
  if (pos.x2 - center.x2 > 0) {
    octant |= 1;
  }
  octant <<= 1;
  if (pos.x3 - center.x3 > 0) {
    octant |= 1;
  }

  return octant;
}

vec3 octree_child_center(const struct octree_node node,
                         const unsigned int octant) {
  double len = node.edge_length * 0.25;
  vec3 center_delta = {0};
  /* x1 difference */
  if (octant & 4) {
    center_delta.x1 += len;
  } else {
    center_delta.x1 -= len;
  }
  /* x2 difference */
  if (octant & 2) {
    center_delta.x2 += len;
  } else {
    center_delta.x2 -= len;
  }
  /* x3 difference */
  if (octant & 1) {
    center_delta.x3 += len;
  } else {
    center_delta.x3 -= len;
  }
  return vec3_sum(node.center, center_delta);
}

unsigned int octree_subdevide(struct octree* self, const unsigned int leaf) {
  const unsigned int octant =
      octree_octant(self->stack[leaf].center, self->stack[leaf].center_of_mass);
  /* Copy and adjust size and center */
  self->stack[self->number_nodes] = self->stack[leaf];
  self->stack[self->number_nodes].center =
      octree_child_center(self->stack[leaf], octant);
  self->stack[self->number_nodes].edge_length =
      self->stack[leaf].edge_length * 0.5;

  /* Zero mass information and set child */
  self->stack[leaf].mass = 0;
  self->stack[leaf].center_of_mass = (vec3){0};
  self->stack[leaf].children[octant] = self->number_nodes;
  self->number_nodes++;

  return octant;
}

void octree_insert(struct octree* self, struct octree_node node) {
  unsigned int index = 0;
  unsigned int octant =
      octree_octant(self->stack[0].center, node.center_of_mass);

  while (self->stack[index].children[octant]) {
    index = self->stack[index].children[octant];
    octant = octree_octant(self->stack[index].center, node.center_of_mass);
  }

  if (octree_is_leaf(self, index)) {
    unsigned int leaf_oct;
    do {
      leaf_oct = octree_subdevide(self, index);
      octant = octree_octant(self->stack[index].center, node.center_of_mass);
      if (octant == leaf_oct) {
        index = self->number_nodes - 1;
      }
    } while (octant == leaf_oct);
  }

  node.parent = index;
  node.center = octree_child_center(self->stack[index], octant);
  node.edge_length = self->stack[index].edge_length * 0.5;
  self->stack[self->number_nodes] = node;
  self->stack[index].children[octant] = self->number_nodes;
  self->number_nodes++;
}

void octree_delete(struct octree* self, unsigned int index) {
  unsigned int parent = self->stack[index].parent;
  for (int i = 0; i < 8; ++i) {
    if (self->stack[parent].children[i] == index) {
      self->stack[parent].children[i] = 0;
    }
  }
  self->stack[index] = (struct octree_node){0};
}

void octree_propagate(struct octree* self) {
  for (unsigned int i = self->number_nodes; i > 0; --i) {
    if (octree_is_leaf(self, i - 1)) {
      continue;
    }

    for (unsigned int j = 0; j < 8; ++j) {
      if (!self->stack[i - 1].children[j]) {
        continue;
      }

      self->stack[i - 1].mass +=
          self->stack[self->stack[i - 1].children[j]].mass;
      self->stack[i - 1].center_of_mass = vec3_sum(
          self->stack[i - 1].center_of_mass,
          vec3_sp(self->stack[self->stack[i - 1].children[j]].mass,
                  self->stack[self->stack[i - 1].children[j]].center_of_mass));
    }
    self->stack[i - 1].center_of_mass =
        vec3_sp(1 / self->stack[i - 1].mass, self->stack[i - 1].center_of_mass);
  }
}

vec3 octree_accel_subtree(struct octree* self, struct sim_state* state,
                          vec3 pos, unsigned int entry) {
  /* If leaf or satisfies theta < threshhold return accel from center of mass */
  const double theta =
      self->stack[entry].edge_length /
      vec3_abs(vec3_diff(pos, self->stack[entry].center_of_mass));
  if (theta < self->threshhold || octree_is_leaf(self, entry)) {
    if (vec3_abs(vec3_diff(pos, self->stack[entry].center_of_mass)) <
        self->tolerance) {
      return (vec3){0};
    }

    vec3 diff = vec3_diff(self->stack[entry].center_of_mass, pos);
    double c = state->g * self->stack[entry].mass /
               (pow(vec3_abs(diff), 3) + state->norm);
    return vec3_sp(c, diff);
  }

  /* If not calculate acceleration for each subtree that is not empty */
  vec3 force = {0};
  for (unsigned int i = 0; i < 8; ++i) {
    if (!self->stack[entry].children[i]) {
      continue;
    }
    force =
        vec3_sum(force, octree_accel_subtree(self, state, pos,
                                             self->stack[entry].children[i]));
  }
  return force;
}

vec3 octree_accel(struct octree* self, struct sim_state* state, vec3 pos) {
  vec3 force = octree_accel_subtree(self, state, pos, 0);
  return force;
}

void octree_cleanup(struct octree* self) { free(self->stack); }
