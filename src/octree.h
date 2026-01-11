#ifndef OCTREE_H
#define OCTREE_H

#include "sim_state.h"

#include <libvecmath/vec3.h>
#include <stdint.h>

struct octree_node {
  double mass;
  vec3 center_of_mass;

  vec3 center;
  double edge_length;

  unsigned int children[8];
  unsigned int parent;
};

struct octree {
  unsigned int stack_size;
  struct octree_node* stack;
  unsigned int number_nodes;

  double tolerance;
  double threshhold;
};

struct octree octree_init(unsigned int stack_size, double threshhold,
                          double tolerance);

void octree_set_root(struct octree* self, struct sim_state* state);

unsigned int octree_is_leaf(struct octree* self, unsigned int node);

unsigned int octree_octant(const vec3 center, const vec3 pos);

vec3 octree_child_center(const struct octree_node node,
                         const unsigned int octant);

unsigned int octree_subdevide(struct octree* self, const unsigned int leaf);

void octree_prepare(struct octree* self, struct sim_state* state);

void octree_insert(struct octree* self, struct octree_node node);

void octree_propagate(struct octree* self);

vec3 octree_accel_subtree(struct octree* self, struct sim_state* state,
                          vec3 pos, unsigned int entry);

vec3 octree_accel(struct octree* self, struct sim_state* state, vec3 pos);

void octree_cleanup(struct octree* self);

#endif /* OCTREE_H */
