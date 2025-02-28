import numpy as np
import random

class VolcanoCrossingMDP:
    def __init__(self, rows, cols, rewards, discount_factor=0.9):
        self.rows = rows
        self.cols = cols
        self.rewards = rewards  # Dictionary with coordinates as keys and rewards as values
        self.discount_factor = discount_factor
        self.actions = ['up', 'down', 'left', 'right']
        self.action_map = {
            'up': (-1, 0),
            'down': (1, 0),
            'left': (0, -1),
            'right': (0, 1)
        }
        
    def is_valid_state(self, state):
        x, y = state
        return 0 <= x < self.rows and 0 <= y < self.cols

    def step(self, state, action):
        dx, dy = self.action_map[action]
        x, y = state
        next_state = (x + dx, y + dy)
        if not self.is_valid_state(next_state):
            next_state = state  # Bump into walls stays in the same state
        reward = self.rewards.get(next_state, 0)
        return next_state, reward

# Define the grid and rewards
rows, cols = 5, 5
rewards = {
    (2, 2): -10,  # Lava lake
    (0, 4): 10,   # Fabulous view
    (4, 4): 5     # Normal view
}

# Initialize the MDP
mdp = VolcanoCrossingMDP(rows, cols, rewards)

# (i) Implement a Random Policy
def generate_random_policy(mdp):
    policy = {}
    for x in range(mdp.rows):
        for y in range(mdp.cols):
            state = (x, y)
            policy[state] = random.choice(mdp.actions)  # Assign a random action
    return policy

# (ii) Policy Evaluation Algorithm
def policy_evaluation(policy, mdp, theta=1e-6):
    V = np.zeros((mdp.rows, mdp.cols))  # Initialize value function array
    while True:
        delta = 0
        for x in range(mdp.rows):
            for y in range(mdp.cols):
                state = (x, y)
                action = policy[state]
                next_state, reward = mdp.step(state, action)
                v = reward + mdp.discount_factor * V[next_state]
                delta = max(delta, abs(v - V[state]))
                V[state] = v
        if delta < theta:
            break
    return V

# (iii) Value Iteration Algorithm
def value_iteration(mdp, theta=1e-6):
    V = np.zeros((mdp.rows, mdp.cols))
    policy = {(x, y): 'right' for x in range(mdp.rows) for y in range(mdp.cols)}

    while True:
        delta = 0
        for x in range(mdp.rows):
            for y in range(mdp.cols):
                state = (x, y)
                action_values = np.zeros(len(mdp.actions))
                for idx, action in enumerate(mdp.actions):
                    next_state, reward = mdp.step(state, action)
                    action_values[idx] = reward + mdp.discount_factor * V[next_state]
                
                best_action_idx = np.argmax(action_values)
                best_value = action_values[best_action_idx]
                delta = max(delta, abs(best_value - V[state]))
                V[state] = best_value
                policy[state] = mdp.actions[best_action_idx]
        
        if delta < theta:
            break
    return policy, V

# (iv) Demonstration of the Random Policy and Value Iteration

# Generate a random policy
random_policy = generate_random_policy(mdp)

# Evaluate the random policy
random_policy_values = policy_evaluation(random_policy, mdp)
print("Random Policy Evaluation (Value Function):\n", random_policy_values)

# Apply value iteration to find the optimal policy and value function
optimal_policy, optimal_value = value_iteration(mdp)
print("Optimal Policy:\n", optimal_policy)
print("Optimal Value Function:\n", optimal_value)
