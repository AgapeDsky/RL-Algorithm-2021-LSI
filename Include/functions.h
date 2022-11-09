#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <random>
#include "params.h"

/**
 * @brief Function to generate demon & reward LUT. Reward (endgoal) is listed as the last element of loc_array
 * 
 * @param mat input matrix
 * @param loc_array array containing locations of demons and endgoal
 * @param n number of demons+1 (endgoal)
 */
void demon_reward_lookup_generator(int mat[SIZE][SIZE], int* loc_array, int n);

/**
 * @brief Function to compute new q value based on post belief
 * 
 * @param cur_state current state
 * @param new_state new state
 * @param q_table q table containing q values
 * @param demon_reward_LUT LUT for demons and endgoal locations
 * @param maze current maze condition
 */
void update_q_value(int cur_state, int new_state, float q_table[SIZE*SIZE][4], int demon_reward_LUT[SIZE][SIZE], int maze[SIZE][SIZE]);

/**
 * @brief Function to generate reward_punishment based on decision
 * 
 * @param new_state new state
 * @param demon_reward_LUT LUT for demons and endgoal locations
 * @param maze current maze condition
 * @return float reward for taking new_state as decision
 */
float policy_generator(int new_state, int demon_reward_LUT[SIZE][SIZE], int maze[SIZE][SIZE]);

/**
 * @brief Function to compute action to be taken to get to a new state
 * 
 * @param cur_state current state
 * @param q_table q table containing q values
 * @param epsilon biasing value, to create more randomness at the start of the process
 * @return int action as 0, 1, 2, or 3
 */
int compute_action(int cur_state, float q_table[SIZE*SIZE][4], float epsilon);

/**
 * @brief Function to print entire q values from q table
 * 
 * @param q_table q table containing q values
 */
void print_q_value(float q_table[SIZE*SIZE][4]);

/**
 * @brief Function to find q table idx to get from cur state to new state
 * 
 * @param cur_state current state
 * @param new_state new state
 * @return int idx on q_table[cur_state]
 */
int find_q_table_idx (int cur_state, int new_state);

/**
 * @brief Function to find maximum value from a row on a q table
 * 
 * @param arr a single row taken from the q table
 * @return float maximum q value
 */
float find_q_table_max (float arr[4]);

/**
 * @brief Function to find maximum value index from a row on a q table
 * 
 * @param arr a single row taken from the q table
 * @return int maximum index
 */
int find_q_table_max_idx (float arr[4]);

/**
 * @brief Function to find submaximum value index from a row on a q table
 * 
 * @param arr a single row taken from the q table
 * @param rank submaximum value rank. highest rank : 0, lowest rank : 3
 * @return int submaximum index
 */
int find_q_table_submax_idx (float arr[4], int rank);

/**
 * @brief Random float number generator [0, 1], equally distributed
 * 
 * @return float Random value
 */
float random_generator();

/**
 * @brief Function to print best route based on learning process
 * 
 * @param q_table q table containing q values
 * @param starting_point starting point
 * @param end_goal end goal
 */
void print_best_route(float q_table[SIZE*SIZE][4], int starting_point, int end_goal);

#endif