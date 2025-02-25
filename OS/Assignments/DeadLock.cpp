#include <iostream>
#include <vector>

using namespace std;

bool isDeadlocked(int processes, int resources, vector<vector<int>> allocation, vector<vector<int>> request) {
  vector<int> work(resources);
  vector<bool> finish(processes, false);

  // Copy available resources to work
  for (int i = 0; i < resources; i++) {
    work[i] = allocation[0][i]; // Assuming all resources are initially available
  }

  for (int i = 0; i < processes; i++) {
    if (finish[i] == false) {
      bool found = true;
      for (int j = 0; j < resources; j++) {
        if (request[i][j] > work[j]) {
          found = false;
          break;
        }
      }

      if (found) {
        for (int j = 0; j < resources; j++) {
          work[j] += allocation[i][j];
        }
        finish[i] = true;
        i--; // Check again if same process can be finished due to resource availability change
      }
    }
  }

  // Check if all processes are finished
  for (int i = 0; i < processes; i++) {
    if (finish[i] == false) {
      return true; // System is in deadlock
    }
  }

  return false; // No deadlock detected
}