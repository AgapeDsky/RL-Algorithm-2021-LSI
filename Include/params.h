#ifndef PARAM_H
#define PARAM_H

// Maze size
#define SIZE 3

// Learning params
#define alpha 0.5
#define gamma 0.9
#define PUNISHMENT 100.
#define REWARD 100.

// Iteration params
#define EPISODE_THRESHOLD 1000
#define MAX_SEARCH 50

// Starting Point
#define STARTING_POINT 0

// End goal and demons
#define count_val 3
extern int demon_endgoal_loc[count_val];

#endif