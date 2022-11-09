#include "Include/functions.h"
#include "Include/params.h"
#include <cassert>
#include <stdio.h>

int main() {
    // Initialize Q table with randomized value
    float q_table[SIZE*SIZE][4];
    int i = 0;
    for (i = 0; i < SIZE*SIZE; i++ ) {
        q_table[i][0] = random_generator();
        q_table[i][1] = random_generator();
        q_table[i][2] = random_generator();
        q_table[i][3] = random_generator();
    }

    // Initialize demon and reward LUT
    int demon_reward_LUT[SIZE][SIZE];
    for (i = 0; i < SIZE; i++ ) {
        int j = 0;
        for (j = 0; j < SIZE; j++) {
            demon_reward_LUT[i][j] = 0;
        }
    }
    int count = count_val;
    int* loc_array = demon_endgoal_loc;
    demon_reward_lookup_generator(demon_reward_LUT, loc_array, count);

    // Define starting point
    int starting_point = STARTING_POINT;

    // Start learning process
    int episode = 0;

    for (episode = 0; episode < EPISODE_THRESHOLD; episode++) {
        // Compute epsilon
        float epsilon = 1 - episode/EPISODE_THRESHOLD;

        // Create maze
        int maze[SIZE][SIZE];
        for (i = 0; i < SIZE; i++ ) {
            int j = 0;
            for (j = 0; j < SIZE; j++) {
                maze[i][j] = 0;
            }
        }

        // Place agent on the maze
        maze[starting_point/SIZE][starting_point%SIZE] = 1;
        int cur_state = starting_point;
        int next_state = starting_point;

        // Compute process with max search limit for each episodes
        int j = 0;
        for (j = 0; j < MAX_SEARCH; j++) {
            // Get next state
            int action = compute_action(cur_state, q_table, epsilon);
            int next_state = (action == 0) ? cur_state+1 :
                             (action == 1) ? cur_state-SIZE :
                             (action == 2) ? cur_state-1 :
                             (action == 3) ? cur_state+SIZE : -10;

            // Next state error condition
            if (next_state == -10) {
                printf("%d -> %d | %d", cur_state, next_state, action);
                printf("\nERROR NEXT STATE COMPUTATION");
                return 1;
            }

            // New state misscalculated
            bool valid = (next_state/SIZE >= 0) && (next_state/SIZE < SIZE) && (next_state%SIZE >= 0) && (next_state%SIZE < SIZE);
            while (!valid) {
                action = compute_action(cur_state, q_table, epsilon);
                next_state = (action == 0) ? cur_state+1 :
                             (action == 1) ? cur_state-3 :
                             (action == 2) ? cur_state-1 :
                             (action == 3) ? cur_state+3 : -10;
                valid = (next_state/SIZE >= 0) && (next_state/SIZE < SIZE) && (next_state%SIZE >= 0) && (next_state%SIZE < SIZE);
            }

            // By now, the new state is valid
            // Update q value
            update_q_value(cur_state, next_state, q_table, demon_reward_LUT, maze);

            // terminate loop if endgoal has been reached
            if (next_state == loc_array[count-1]) {
                break;
            }

            // update current state
            cur_state = next_state;
            maze[next_state/SIZE][next_state%SIZE] += 1;
        }
    }

    // Learning has been done, print the q table
    print_q_value(q_table);

    // Print the best route
    print_best_route(q_table, starting_point, loc_array[count-1]);

    return 0;
}