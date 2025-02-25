
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>

using namespace std;

struct GanttEntry
{
    int processId;
    int startTime;
    int endTime;
};

void RandomProcess(int arrivalTime[], int burstTime[], int numProcesses)
{
    for (int i = 0; i < numProcesses; ++i)
    {
        arrivalTime[i] = rand() % 11;
        burstTime[i] = rand() % 11 + 1;
    }
}

void sortArrival(int arrivalTime[], int burstTime[], int numProcesses)
{
    for (int i = 0; i < numProcesses - 1; ++i)
    {
        for (int j = i + 1; j < numProcesses; ++j)
        {
            if (arrivalTime[i] > arrivalTime[j])
            {
                swap(arrivalTime[i], arrivalTime[j]);
                swap(burstTime[i], burstTime[j]);
            }
        }
    }
}

void calculateTimes(int arrivalTime[], int burstTime[], int completionTime[], int turnaroundTime[], int waitingTime[], int numProcesses)
{
    int currentTime = 0;
    for (int i = 0; i < numProcesses; ++i)
    {
        if (currentTime < arrivalTime[i])
        {
            currentTime = arrivalTime[i];
        }
        completionTime[i] = currentTime + burstTime[i];
        turnaroundTime[i] = completionTime[i] - arrivalTime[i];
        waitingTime[i] = turnaroundTime[i] - burstTime[i];
        currentTime = completionTime[i];
    }
}

void Averages(int turnaroundTime[], int waitingTime[], int numProcesses)
{
    double totalTAT = 0, totalWT = 0;
    for (int i = 0; i < numProcesses; ++i)
    {
        totalTAT += turnaroundTime[i];
        totalWT += waitingTime[i];
    }
    cout << "\nAverage Turnaround Time: " << totalTAT / numProcesses << endl;
    cout << "Average Waiting Time: " << totalWT / numProcesses << endl;
}

void displayGanttChart(int arrivalTime[], int burstTime[], int numProcesses)
{
    cout << "\nGantt Chart:\n";
    for (int i = 0; i < numProcesses; ++i)
    {
        cout << "|  P" << i + 1 << "  ";
    }
    cout << "|\n";

    int currentTime = 0;
    for (int i = 0; i < numProcesses; ++i)
    {
        if (currentTime < arrivalTime[i])
        {
            currentTime = arrivalTime[i];
        }
        cout << currentTime << "      ";
        currentTime += burstTime[i];
    }
    cout << currentTime << endl;
}

void FCFS_nonPreemptive(int arrivalTime[], int burstTime[], int numProcesses)
{
    sortArrival(arrivalTime, burstTime, numProcesses);
    int completionTime[numProcesses], turnaroundTime[numProcesses], waitingTime[numProcesses];
    calculateTimes(arrivalTime, burstTime, completionTime, turnaroundTime, waitingTime, numProcesses);

    cout << "\nFCFS Scheduling Results:\n";
    cout << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for (int i = 0; i < numProcesses; ++i)
    {
        cout << "P" << i + 1 << "\t" << arrivalTime[i] << "\t" << burstTime[i] << "\t"
             << completionTime[i] << "\t\t" << turnaroundTime[i] << "\t\t" << waitingTime[i] << endl;
    }

    displayGanttChart(arrivalTime, burstTime, numProcesses);
    Averages(turnaroundTime, waitingTime, numProcesses);
}

void SJF_nonPreemitive(int arrivalTime[], int burstTime[], int numProcesses)
{
    int remainingTime[numProcesses];
    int completionTime[numProcesses] = {0};
    int turnaroundTime[numProcesses];
    int waitingTime[numProcesses];
    bool completed[numProcesses] = {false};

    vector<GanttEntry> ganttChart;

    for (int i = 0; i < numProcesses; ++i)
    {
        remainingTime[i] = burstTime[i];
    }

    int currentTime = 0, completedProcesses = 0;

    while (completedProcesses < numProcesses)
    {
        int shortest = -1;
        int minRemainingTime = 21;

        for (int i = 0; i < numProcesses; ++i)
        {
            if (arrivalTime[i] <= currentTime && !completed[i] && remainingTime[i] < minRemainingTime)
            {
                minRemainingTime = remainingTime[i];
                shortest = i;
            }
        }

        if (shortest == -1)
        {
            currentTime++;
            continue;
        }

        if (!ganttChart.empty() && ganttChart.back().processId == shortest + 1)
        {
            ganttChart.back().endTime++;
        }
        else
        {
            ganttChart.push_back({shortest + 1, currentTime, currentTime + 1});
        }

        remainingTime[shortest]--;
        currentTime++;

        if (remainingTime[shortest] == 0)
        {
            completed[shortest] = true;
            completionTime[shortest] = currentTime;
            turnaroundTime[shortest] = completionTime[shortest] - arrivalTime[shortest];
            waitingTime[shortest] = turnaroundTime[shortest] - burstTime[shortest];
            completedProcesses++;
        }
    }

    cout << "\nSJF Preemptive Scheduling Results:\n";
    cout << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for (int i = 0; i < numProcesses; ++i)
    {
        cout << "P" << i + 1 << "\t" << arrivalTime[i] << "\t" << burstTime[i] << "\t"
             << completionTime[i] << "\t\t" << turnaroundTime[i] << "\t\t" << waitingTime[i] << endl;
    }

    Averages(turnaroundTime, waitingTime, numProcesses);

    cout << "\nGantt Chart:\n";
    for (int i = 0; i < ganttChart.size(); ++i)
    {
        cout << "|  P" << ganttChart[i].processId << "  ";
    }
    cout << "|\n";

    for (int i = 0; i < ganttChart.size(); ++i)
    {
        cout << ganttChart[i].startTime << "\t";
    }
    cout << ganttChart.back().endTime << endl;
}

void RoundRobin(int arrivalTime[], int burstTime[], int numProcesses, int quantum)
{
    int remainingTime[numProcesses];
    int completionTime[numProcesses] = {0};
    int turnaroundTime[numProcesses];
    int waitingTime[numProcesses];

    for (int i = 0; i < numProcesses; ++i)
    {
        remainingTime[i] = burstTime[i];
    }

    int currentTime = 0;
    queue<int> processQueue;
    vector<GanttEntry> ganttChart;
    bool isProcessInQueue[numProcesses] = {false};

    for (int i = 0; i < numProcesses; ++i)
    {
        if (arrivalTime[i] <= currentTime)
        {
            processQueue.push(i);
            isProcessInQueue[i] = true;
        }
    }

    while (!processQueue.empty())
    {
        int i = processQueue.front();
        processQueue.pop();

        int startTime = currentTime;
        if (remainingTime[i] > quantum)
        {
            currentTime += quantum;
            remainingTime[i] -= quantum;
        }
        else
        {
            currentTime += remainingTime[i];
            remainingTime[i] = 0;
            completionTime[i] = currentTime;
        }
        ganttChart.push_back({i + 1, startTime, currentTime});

        for (int j = 0; j < numProcesses; ++j)
        {
            if (!isProcessInQueue[j] && arrivalTime[j] <= currentTime && remainingTime[j] > 0)
            {
                processQueue.push(j);
                isProcessInQueue[j] = true;
            }
        }

        if (remainingTime[i] > 0)
        {
            processQueue.push(i);
        }
    }

    for (int i = 0; i < numProcesses; ++i)
    {
        turnaroundTime[i] = completionTime[i] - arrivalTime[i];
        waitingTime[i] = turnaroundTime[i] - burstTime[i];
    }

    cout << "\nRound Robin Scheduling Results:\n";
    cout << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for (int i = 0; i < numProcesses; ++i)
    {
        cout << "P" << i + 1 << "\t" << arrivalTime[i] << "\t" << burstTime[i] << "\t"
             << completionTime[i] << "\t\t" << turnaroundTime[i] << "\t\t" << waitingTime[i] << endl;
    }

    Averages(turnaroundTime, waitingTime, numProcesses);
}

int main()
{
    int numProcesses, choice, quantum;

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    int arrivalTime[numProcesses], burstTime[numProcesses];
    srand(time(0));
    RandomProcess(arrivalTime, burstTime, numProcesses);

    do
    {
        cout << "\nSelect Scheduling Algorithm:\n";
        cout << "1. FCFS\n2. SJF Preemptive\n3. Round Robin\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            FCFS_nonPreemptive(arrivalTime, burstTime, numProcesses);
            break;
        case 2:
            SJF_nonPreemitive(arrivalTime, burstTime, numProcesses);
            break;
        case 3:
            cout << "Enter time quantum for Round Robin: ";
            cin >> quantum;
            RoundRobin(arrivalTime, burstTime, numProcesses, quantum);
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
