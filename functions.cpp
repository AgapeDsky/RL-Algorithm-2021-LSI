#include "Include/functions.h"
#include <stdio.h>

void demon_reward_lookup_generator(int mat[SIZE][SIZE], int* loc_array, int n) {
    // Generate demons, indexed as 1
    int i = 0;
    for (i = 0; i < n-1; i++) {
        int loc = loc_array[i];
        mat[loc/SIZE][loc%SIZE] = 1;
    }

    // Generate endgoal reward, indexed as 2
    mat[loc_array[n-1]/SIZE][loc_array[n-1]%SIZE] = 2;

    return;
}

void update_q_value(int cur_state, int new_state, float q_table[SIZE*SIZE][4], int demon_reward_LUT[SIZE][SIZE], int maze[SIZE][SIZE]) {
    // Get max q value on a state in the q table
    float max_q_table_new_state = find_q_table_max(q_table[new_state]);
    
    // Compute q table column location based on the new state and cur state
    int q_table_idx = find_q_table_idx(cur_state, new_state);

    // Update q value
    q_table[cur_state][q_table_idx] = (1-alpha) * q_table[cur_state][q_table_idx] +
                                      alpha * (policy_generator(new_state, demon_reward_LUT, maze) + gamma * max_q_table_new_state);

    return;
}

void print_q_value(float q_table[SIZE*SIZE][4]) {
    int i = 0;
    for (i = 0; i < SIZE*SIZE; i++) {
        printf("State: %d\t", i);
        int j = 0;
        for (j = 0; j < 4; j++) {
            printf("%f\t", q_table[i][j]);
        }
        printf("\n");
    }
}

void print_best_route(float q_table[SIZE*SIZE][4], int starting_point, int end_goal) {
    // Add starting point
    int cur_state = starting_point;
    printf("Most efficient path :\n");

    // Iterate while end goal is yet to be reached
    while (cur_state != end_goal) {
        printf("S%d->", cur_state);
        int action;
        int next_state;
        bool valid = 0;

        int i = 0;
        // Compute only valid action
        while (!valid) {
            // Compute action
            int cur_row = cur_state/SIZE;
            int cur_col = cur_state%SIZE;
            action = find_q_table_submax_idx(q_table[cur_state], i);

            // Check action validity
            if (action == 0 && cur_col == 4) {}
            else if (action == 1 && cur_row == 0) {}
            else if (action == 2 && cur_col == 0) {}
            else if (action == 3 && cur_row == 4) {}
            else {
                valid = 1;
            }

            // If action is already valid, compute next_state
            if (valid) {
                // printf("action: %d\n", action);
                next_state = (action == 0) ? cur_state+1    :
                             (action == 1) ? cur_state-SIZE :
                             (action == 2) ? cur_state-1    :
                             (action == 3) ? cur_state+SIZE : -10;
            }
            i++;
        }
        cur_state = next_state;
    }
    printf("S%d", cur_state);
}

int compute_action(int cur_state, float q_table[SIZE*SIZE][4], float epsilon) {
    // Get a random value
    float random = random_generator();
    int res = 0;
    bool valid = 0; bool prev_belief_invalid = 0;

    while (!valid) {
        if (epsilon > random || prev_belief_invalid) {
            // If epsilon > random value, take action based on probability
            res = rand()%4;
        }
        else {
            // Else, take action based on previous belief
            res = find_q_table_max_idx(q_table[cur_state]);
            prev_belief_invalid = 1;
        }
        int cur_row = cur_state/SIZE;
        int cur_col = cur_state%SIZE;
        if (res == 0 && cur_col == 4) {}
        else if (res == 1 && cur_row == 0) {}
        else if (res == 2 && cur_col == 0) {}
        else if (res == 3 && cur_row == 4) {}
        else {
            valid = 1;
        }
    }

    return res;
}

float find_q_table_max (float arr[4]) {
    float res = 0;
    int i = 0;
    for (i = 0; i < 4; ++i) {
        if (arr[i] > res) {
            res = arr[i];
        }
    }
    return res;
}

int find_q_table_max_idx (float arr[4]) {
    int ret = 0;
    float res = 0;
    int i = 0;
    for (i = 0; i < 4; ++i) {
        if (arr[i] > res) {
            res = arr[i];
            ret = i;
        }
    }
    return ret;
}

int find_q_table_submax_idx (float arr[4], int rank) {
    struct float_tuple {
        float a; float b;
    };
    float_tuple sorted[4];
    sorted[0] = {arr[0], 0}; sorted[1] = {arr[1], 1};
    sorted[2] = {arr[2], 2}; sorted[3] = {arr[3], 3};

    int i, j;
    for (i = 0; i < 4-1; i++) {
 
        // Last i elements are already in place
        for (j = 0; j < 4 - i - 1; j++) {
            if (sorted[j].a < sorted[j + 1].a) {
                float_tuple temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }

    return sorted[rank].b;
}

int find_q_table_idx (int cur_state, int new_state) {
    // Get cur state and next state cols n rows
    int cur_row = cur_state / SIZE;
    int cur_col = cur_state % SIZE;
    int new_row = new_state / SIZE;
    int new_col = new_state % SIZE;

    // Get action based on the cur state and new state
    if (new_row - cur_row == 1) {
        return 3;
    }
    else if (cur_row - new_row == 1) {
        return 1;
    }
    else if (new_col - cur_col == 1) {
        return 0;
    }
    else if (cur_col - new_col == 1) {
        return 2;
    }
    else {
        return -1; // not adjacent, algorithm failure
    }
}

float policy_generator(int new_state, int demon_reward_LUT[SIZE][SIZE], int maze[SIZE][SIZE]) {
    float reward = 0;
    bool valid = (new_state/SIZE >= 0) && (new_state/SIZE < SIZE) && (new_state%SIZE >= 0) && (new_state%SIZE < SIZE);
    if (!valid) {
        return -PUNISHMENT;
    }

    // Calculate punishment and reward from demons n endgoal
    if (demon_reward_LUT[new_state/SIZE][new_state%SIZE] == 1) {
        reward -= PUNISHMENT;
    }
    else if (demon_reward_LUT[new_state/SIZE][new_state%SIZE] == 2) {
        reward += REWARD;
    }

    // Calculate punishment for taking a tile more than once
    reward -= 10 * maze[new_state/SIZE][new_state%SIZE];
    
    return reward;
}

float random_generator() {
    float r = (float)(rand()) /(float)(RAND_MAX);
    return r;
}