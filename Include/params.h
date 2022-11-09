#ifndef PARAM_H
#define PARAM_H

// Maze size
#define SIZE 5

// Learning params
#define alpha 0.5
#define gamma 0.9
#define PUNISHMENT 100.
#define REWARD 100.

// Iteration params
#define EPISODE_THRESHOLD 3000
#define MAX_SEARCH 50

// Starting Point
#define STARTING_POINT 0

// End goal and demons
#define count_val 5
extern int demon_endgoal_loc[count_val];

#endif