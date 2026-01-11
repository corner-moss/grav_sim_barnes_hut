#ifndef SIM_TIME_H
#define SIM_TIME_H

#include <sys/time.h>

#define MUS_PER_S 1e6

typedef struct sim_time {
  struct timezone tz;
  struct timeval t0;   // Init time
  struct timeval t_lf; // Timestamp of last frame
  struct timeval t_lt; // Timestamp of last tick
} sim_time;

void sim_time_init(sim_time *self);

/* Get time since tx in micro seconds */
double sim_time_dt_0(sim_time *self);
double sim_time_dt_lf(sim_time *self);
double sim_time_dt_lt(sim_time *self);

/* Set tx to current time */
void sim_time_lf_now(sim_time *self);
void sim_time_lt_now(sim_time *self);

#endif /* Ends SIM_TIME_H include guard */
