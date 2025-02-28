import numpy as np

# (i) Define the problem as an MDP

class VolcanoCrossingMDP:
    def _init_(self, rows, cols, rewards, discount_factor=0.9):
        self.rows = rows
        self.cols = cols
        self.rewards = rewards  # Dictionary with coordinates as keys and rewards as values
        self.discount_factor = discount_factor
        self.actions = ['up', 'down', 'left', 'right']
        
    def is_valid_state(self, state):
        x, y = state
        return 0 <= x < self.rows and 0 <= y < self.cols

    def step(self, state, action):
        x, y = state
        if action == 'up':
            next_state = (x - 1, y)
        elif action == 'down':
            next_state = (x + 1, y)
        elif action == 'left':
            next_state = (x, y - 1)
        elif action == 'right':
            next_state = (x, y + 1)
        else:
            raise ValueError("Invalid action")

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

# (ii) Policy Evaluation Algorithm
def policy_evaluation(policy, mdp, theta=1e-6):
    V = np.zeros((mdp.rows, mdp.cols))
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

# Sample policy for evaluation
policy = {(x, y): 'right' for x in range(rows) for y in range(cols)}
V = policy_evaluation(policy, mdp)
print("Policy Evaluation (Value Function):\n", V)

# (iii) Value Iteration Algorithm
def value_iteration(mdp, theta=1e-6):
    V = np.zeros((mdp.rows, mdp.cols))
    policy = {(x, y): None for x in range(mdp.rows) for y in range(mdp.cols)}

    while True:
        delta = 0
        for x in range(mdp.rows):
            for y in range(mdp.cols):
                state = (x, y)
                action_values = {}
                for action in mdp.actions:
                    next_state, reward = mdp.step(state, action)
                    action_values[action] = reward + mdp.discount_factor * V[next_state]
                best_action = max(action_values, key=action_values.get)
                best_value = action_values[best_action]
                delta = max(delta, abs(best_value - V[state]))
                V[state] = best_value
                policy[state] = best_action
        if delta < theta:
            break
    return policy, V

# Find the optimal policy and value function
optimal_policy, optimal_value = value_iteration(mdp)
print("Optimal Policy:\n", optimal_policy)
print("Optimal Value Function:\n", optimal_value)