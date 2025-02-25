#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int max_size = 5;
const int max_resources = 5;

struct processor
{
    int number;
    int allocation[max_resources];
    int maximum[max_resources];
    int request[max_resources];
    int need[max_resources];
    bool flag;
};

void prompt_and_initialize_data(processor p[], int &p_size, int &r_size, int available[])
{
    srand(time(0));

    do
    {
        cout << "Enter number of processes (1-" << max_size << "): ";
        cin >> p_size;
    } while (p_size < 1 || p_size > max_size);

    do
    {
        cout << "Enter number of resource types (1-" << max_resources << "): ";
        cin >> r_size;
    } while (r_size < 1 || r_size > max_resources);

    for (int i = 0; i < r_size; i++)
    {
        available[i] = rand() % 6 + 17;
    }

    for (int i = 0; i < p_size; i++)
    {
        p[i].number = i + 1;
        p[i].flag = false;

        for (int j = 0; j < r_size; j++)
        {
            p[i].allocation[j] = rand() % 5;
            p[i].maximum[j] = rand() % 6 + 5;
            p[i].request[j] = p[i].maximum[j] - p[i].allocation[j];
            p[i].need[j] = p[i].maximum[j] - p[i].allocation[j];
            available[j] -= p[i].allocation[j];
            if (available[j] < 0)
                available[j] = 0;
        }
    }
}

bool safety_algorithm(processor p[], int p_size, int r_size, int available[])
{
    int work[max_resources];
    bool finish[max_size] = {false};
    int safe_sequence[max_size];
    int safe_count = 0;

    for (int i = 0; i < r_size; i++)
    {
        work[i] = available[i];
    }

    while (true)
    {
        bool progress = false;

        for (int i = 0; i < p_size; i++)
        {
            if (!finish[i])
            {
                bool can_satisfy = true;

                for (int j = 0; j < r_size; j++)
                {
                    if (p[i].need[j] > work[j])
                    {
                        can_satisfy = false;
                        break;
                    }
                }

                if (can_satisfy)
                {
                    for (int j = 0; j < r_size; j++)
                    {
                        work[j] += p[i].allocation[j];
                    }
                    finish[i] = true;
                    p[i].flag = true;
                    safe_sequence[safe_count++] = p[i].number;
                    progress = true;
                }
            }
        }

        if (!progress)
            break;
    }

    for (int i = 0; i < p_size; i++)
    {
        if (!finish[i])
        {
            cout << "\nThe system is not in a safe state.\n";
            return false;
        }
    }

    cout << "\nThe system is in a safe state.\nSafe sequence: ";
    for (int i = 0; i < safe_count; i++)
    {
        cout << "P" << safe_sequence[i];
        if (i < safe_count - 1)
            cout << " -> ";
    }
    cout << "\n";

    return true;
}

void detection_algorithm(processor p[], int p_size, int r_size, int available[])
{
    int work[max_resources];
    bool finish[max_size] = {false};

    for (int i = 0; i < r_size; i++)
    {
        work[i] = available[i];
    }

    for (int i = 0; i < p_size; i++)
    {
        bool is_zero_allocation = true;
        for (int j = 0; j < r_size; j++)
        {
            if (p[i].allocation[j] != 0)
            {
                is_zero_allocation = false;
                break;
            }
        }
        finish[i] = is_zero_allocation;
    }

    while (true)
    {
        bool progress = false;

        for (int i = 0; i < p_size; i++)
        {
            if (!finish[i])
            {
                bool can_satisfy = true;

                for (int j = 0; j < r_size; j++)
                {
                    if (p[i].request[j] > work[j])
                    {
                        can_satisfy = false;
                        break;
                    }
                }

                if (can_satisfy)
                {
                    for (int j = 0; j < r_size; j++)
                    {
                        work[j] += p[i].allocation[j];
                    }
                    finish[i] = true;
                    progress = true;
                }
            }
        }

        if (!progress)
            break;
    }

    bool deadlock = false;
    cout << "\nDeadlocked processes: ";
    for (int i = 0; i < p_size; i++)
    {
        if (!finish[i])
        {
            deadlock = true;
            cout << "P" << p[i].number << " ";
        }
    }

    if (!deadlock)
    {
        cout << "None\n";
    }
    else
    {
        cout << "\nThe system is in a deadlock state.\n";
    }
}

void print_data(const processor p[], int p_size, int r_size, const int available[], bool is_request)
{
    cout << "\nProcessor\tAllocation\t\t" << (is_request ? "Request" : "Maximum") << "\t\tNeed\t\tAvailable\n";
    cout << "---------------------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < p_size; i++)
    {
        cout << "P" << p[i].number << "\t\t";

        for (int j = 0; j < r_size; j++)
            cout << setw(3) << p[i].allocation[j] << " ";
        cout << "\t";

        for (int j = 0; j < r_size; j++)
            cout << setw(3) << (is_request ? p[i].request[j] : p[i].maximum[j]) << " ";
        cout << "\t";

        for (int j = 0; j < r_size; j++)
            cout << setw(3) << p[i].need[j] << " ";

        if (i == 0)
        {
            cout << "\t";
            for (int j = 0; j < r_size; j++)
                cout << setw(3) << available[j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    processor p[max_size];
    int p_size, r_size;
    int available[max_resources];

    while (true)
    {
        int choice;
        cout << "\n1. Safety Algorithm\n2. Deadlock Detection Algorithm\n3. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            prompt_and_initialize_data(p, p_size, r_size, available);
            cout << "\n---------------------------------------------------------------------------------------------------------------------\n";
            if (safety_algorithm(p, p_size, r_size, available))
            {
                print_data(p, p_size, r_size, available, false);
            }
            cout << "---------------------------------------------------------------------------------------------------------------------\n";
            break;

        case 2:
            prompt_and_initialize_data(p, p_size, r_size, available);
            cout << "\n---------------------------------------------------------------------------------------------------------------------\n";
            detection_algorithm(p, p_size, r_size, available);
            print_data(p, p_size, r_size, available, true);
            cout << "---------------------------------------------------------------------------------------------------------------------\n";
            break;

        case 3:
            return 0;

        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
