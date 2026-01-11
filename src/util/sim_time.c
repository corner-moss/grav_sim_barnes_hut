#include "sim_time.h"

#include <sys/time.h>

double dt(struct timeval *t1, struct timeval *t0) {
  double dt =
      MUS_PER_S * (t1->tv_sec - t0->tv_sec) + (t1->tv_usec - t0->tv_usec);
  return dt;
}

void sim_time_init(sim_time *self) {
  struct timezone tz;
  self->tz = tz;
  struct timeval current;
  gettimeofday(&current, &(self->tz));

  self->t0 = current;
  self->t_lt = current;
  self->t_lf = current;
}

double sim_time_dt_0(sim_time *self) {
  struct timeval current;
  gettimeofday(&current, &(self->tz));
  double delta = dt(&current, &(self->t0));

  return delta;
}

double sim_time_dt_lf(sim_time *self) {
  struct timeval current;
  gettimeofday(&current, &(self->tz));
  double delta = dt(&current, &(self->t_lf));

  return delta;
}

double sim_time_dt_lt(sim_time *self) {
  struct timeval current;
  gettimeofday(&current, &(self->tz));
  double delta = dt(&current, &(self->t_lt));

  return delta;
}

void sim_time_lf_now(sim_time *self) { gettimeofday(&self->t_lf, &(self->tz)); }

void sim_time_lt_now(sim_time *self) { gettimeofday(&self->t_lt, &(self->tz)); }
