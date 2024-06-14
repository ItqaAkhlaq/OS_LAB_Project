#include <windows.h>
#include <iostream>
//#include <iomanip> // For setw()
#include <sstream>
#include <cstring>
#include <stdlib.h> // For atoi

#define MAX_PROCESSES 100

using namespace std;

template <typename T>
string ToString(T val) {
    ostringstream stream;
    stream << val;
    return stream.str();
}

// Define global variables to hold handles to input fields
HWND hwndArrivalInput;
HWND hwndBurstInput;
HWND hwndPriorityInput;
HWND hwndQuantumInput;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
};

class Scheduler {
private:
    Process processes[MAX_PROCESSES];
    int processCount;
    int timeQuantum;
    // Variables for Gantt chart drawing
    HDC hdc;
    RECT clientRect;
    int chartTopMargin;
    int chartLeftMargin;
    int chartWidth;
    int chartHeight;
    COLORREF colors[MAX_PROCESSES];

public:
    Scheduler() : processCount(0), timeQuantum(1) {}

    void AddProcess(int arrivalTime, int burstTime, int priority) {
        if (processCount >= MAX_PROCESSES) {
            MessageBox(NULL, "Process limit reached!", "Error", MB_OK | MB_ICONERROR);
            return;
        }

        Process p;
        p.id = processCount + 1;
        p.arrivalTime = arrivalTime;
        p.burstTime = burstTime;
        p.priority = priority;

        processes[processCount++] = p;
        //MessageBox(NULL, "Process Added!", "Info", MB_OK);
        MessageBox(NULL, ("Process Added! Total Processes: " + ToString(processCount)).c_str(), "Info", MB_OK);
    }

    void SetTimeQuantum(int quantum) {
        timeQuantum = quantum;
    }

    void SimulateScheduling(void (Scheduler::*scheduler)(Process[], int)) {
        (this->*scheduler)(processes, processCount);
    }
    void DrawGanttChart(); // Method declaration
    void FCFS(Process processes[], int count);
    void RoundRobin(Process processes[], int count);
    void SJN(Process processes[], int count);
    void ShortestRemainingJobFirst(Process processes[], int count);

    void PriorityScheduling(Process processes[], int count);

private:
    void DisplayResult(float avgWaitingTime, float avgTurnaroundTime, const string& algorithmName) {
        string result = "Average Waiting Time: " + ToString(avgWaitingTime) + "\n";
        result += "Average Turnaround Time: " + ToString(avgTurnaroundTime) + "\n";

        result += "Process Information:\n";
        for (int i = 0; i < processCount; ++i) {
            result += "Process " + ToString(processes[i].id) + " - Arrival Time: " + ToString(processes[i].arrivalTime) +
                      ", Burst Time: " + ToString(processes[i].burstTime) +
                      ", Waiting Time: " + ToString(processes[i].waitingTime) +
                      ", Turnaround Time: " + ToString(processes[i].turnaroundTime) + "\n";
        }

        MessageBox(NULL, result.c_str(), (algorithmName + " Simulation Result").c_str(), MB_OK);
    }

    /*void DisplayResult(float avgWaitingTime, float avgTurnaroundTime, const string& algorithmName) {
        string result = "Average Waiting Time: " + ToString(avgWaitingTime) + "\n";
        result += "Average Turnaround Time: " + ToString(avgTurnaroundTime) + "\n";
        MessageBox(NULL, result.c_str(), (algorithmName + " Simulation Result").c_str(), MB_OK);
    }*/


};

void Scheduler::DrawGanttChart() {
    hdc = GetDC(NULL);
    GetClientRect(GetDesktopWindow(), &clientRect);
    int currentX = chartLeftMargin;
    int currentY = chartTopMargin;
    int unitWidth = chartWidth / processCount;

    for (int i = 0; i < processCount; ++i) {
        int processWidth = processes[i].burstTime * unitWidth / timeQuantum;
        HBRUSH hBrush = CreateSolidBrush(colors[i]);
        RECT rect = { currentX, currentY, currentX + processWidth, currentY + chartHeight };
        FillRect(hdc, &rect, hBrush);
        currentX += processWidth;
        DeleteObject(hBrush);
    }
    ReleaseDC(NULL, hdc);
}

void Scheduler::FCFS(Process processes[], int count) {
    // Sort processes by arrival time
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    // Declare chartStream variable to store the Gantt chart
    ostringstream chartStream;

    chartStream << "Gantt Chart for FCFS:\n";
    chartStream << "----------------------------------------\n";
    chartStream << "| Process | Arrival | Start   | End     |\n";
    chartStream << "----------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        if (processes[i].arrivalTime > currentTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        totalWaitingTime += processes[i].waitingTime;
        int startTime = currentTime;
        int endTime = startTime + processes[i].burstTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
        chartStream << "|    " << processes[i].id << "    |   " << processes[i].arrivalTime << "   |    " << startTime << "     |     " << endTime << "       |\n";
        currentTime += processes[i].burstTime;
    }
    chartStream << "----------------------------------------\n";

    float avgWaitingTime = (float)totalWaitingTime / count;
    float avgTurnaroundTime = (float)totalTurnaroundTime / count;
    DisplayResult(avgWaitingTime, avgTurnaroundTime, "FCFS");

    // Show the Gantt chart in a message box
    MessageBox(NULL, chartStream.str().c_str(), "Gantt Chart for FCFS", MB_OK);
}


void Scheduler::RoundRobin(Process processes[], int count) {
    // Sort processes by arrival time
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    Process readyQueue[MAX_PROCESSES];
    int front = 0, rear = -1;
    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int currentIndex = 0;

    // Declare chartStream variable to store the Gantt chart
    ostringstream chartStream;

    chartStream << "Gantt Chart for Round Robin:\n";
    chartStream << "----------------------------------------\n";
    chartStream << "| Time | Process |\n";
    chartStream << "----------------------------------------\n";

    while (front <= rear || currentIndex < count) {
        while (currentIndex < count && processes[currentIndex].arrivalTime <= currentTime) {
            readyQueue[++rear] = processes[currentIndex++];
        }

        if (front > rear) {
            currentTime = processes[currentIndex].arrivalTime;
            continue;
        }

        Process currentProcess = readyQueue[front++];
        int startTime = currentTime;
        int executeTime = currentProcess.burstTime > timeQuantum ? timeQuantum : currentProcess.burstTime;
        currentProcess.burstTime -= executeTime;
        currentTime += executeTime;

        // Record when CPU is given to the process
        chartStream << "| " << startTime << "   |    P" << currentProcess.id << "    |\n";
        if (currentProcess.burstTime > 0) {
            readyQueue[++rear] = currentProcess;
        } else {
            // Record when CPU is taken back from the process
            chartStream << "| " << currentTime << "   |    ----    |\n";

            currentProcess.waitingTime = currentTime - currentProcess.arrivalTime - executeTime;
            currentProcess.turnaroundTime = currentTime - currentProcess.arrivalTime;
            totalWaitingTime += currentProcess.waitingTime;
            totalTurnaroundTime += currentProcess.turnaroundTime;
        }
    }

    chartStream << "----------------------------------------\n";

    float avgWaitingTime = (float)totalWaitingTime / count;
    float avgTurnaroundTime = (float)totalTurnaroundTime / count;
    DisplayResult(avgWaitingTime, avgTurnaroundTime, "Round Robin");

    // Show the Gantt chart in a message box
    MessageBox(NULL, chartStream.str().c_str(), "Gantt Chart for Round Robin", MB_OK);
}
void Scheduler::SJN(Process processes[], int count) {
    // Sort processes by burst time
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (processes[j].burstTime > processes[j + 1].burstTime) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    // Declare chartStream variable to store the Gantt chart
    ostringstream chartStream;

    chartStream << "Gantt Chart for SJN:\n";
    chartStream << "----------------------------------------\n";
    chartStream << "| Time | Process |\n";
    chartStream << "----------------------------------------\n";

    for (int i = 0; i < count; ++i) {
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        totalWaitingTime += processes[i].waitingTime;

        // Record when CPU is given to the process
        chartStream << "| " << currentTime << "   |    P" << processes[i].id << "    |\n";

        currentTime += processes[i].burstTime;

        // Record when CPU is taken back from the process
        chartStream << "| " << currentTime << "   |    ----    |\n";

        processes[i].turnaroundTime = currentTime - processes[i].arrivalTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    chartStream << "----------------------------------------\n";

    float avgWaitingTime = (float)totalWaitingTime / count;
    float avgTurnaroundTime = (float)totalTurnaroundTime / count;
    DisplayResult(avgWaitingTime, avgTurnaroundTime, "SJN");

    // Show the Gantt chart in a message box
    MessageBox(NULL, chartStream.str().c_str(), "Gantt Chart for SJN", MB_OK);
}


void Scheduler::ShortestRemainingJobFirst(Process processes[], int count) {
    // Manual sorting of processes by arrival time
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int currentTime = 0;
    int completed = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    bool visited[MAX_PROCESSES] = { false };

    ostringstream chartStream;
    chartStream << "Gantt Chart for SRJF:\n";
    chartStream << "----------------------------------------\n";
    chartStream << "| Process | Start Time | End Time |\n";
    chartStream << "----------------------------------------\n";

    while (completed < count) {
        int idx = -1;
        int minBurst = INT_MAX;

        for (int i = 0; i < count; ++i) {
            if (processes[i].arrivalTime <= currentTime && !visited[i] && processes[i].burstTime < minBurst) {
                minBurst = processes[i].burstTime;
                idx = i;
            }
        }

        if (idx != -1) {
            visited[idx] = true;
            int startTime = currentTime;
            currentTime += processes[idx].burstTime;
            int endTime = currentTime;

            processes[idx].waitingTime = startTime - processes[idx].arrivalTime;
            processes[idx].turnaroundTime = endTime - processes[idx].arrivalTime;

            totalWaitingTime += processes[idx].waitingTime;
            totalTurnaroundTime += processes[idx].turnaroundTime;

            chartStream << "|    " << processes[idx].id << "    |    " << startTime << "    |    " << endTime << "    |\n";

            completed++;
        } else {
            currentTime++;
        }
    }

    chartStream << "----------------------------------------\n";

    float avgWaitingTime = (float)totalWaitingTime / count;
    float avgTurnaroundTime = (float)totalTurnaroundTime / count;
    DisplayResult(avgWaitingTime, avgTurnaroundTime, "SRJF");

    MessageBox(NULL, chartStream.str().c_str(), "Gantt Chart for SRJF", MB_OK);
}


void Scheduler::PriorityScheduling(Process processes[], int count) {
    // Sort processes by priority
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (processes[j].priority < processes[j + 1].priority) { // Higher priority first
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    // Declare chartStream variable to store the Gantt chart
    ostringstream chartStream;

    chartStream << "Gantt Chart for Priority Scheduling:\n";
    chartStream << "----------------------------------------\n";
    chartStream << "| Time | Process |\n";
    chartStream << "----------------------------------------\n";

    for (int i = 0; i < count; ++i) {
        if (processes[i].arrivalTime > currentTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        totalWaitingTime += processes[i].waitingTime;

        // Record when CPU is given to the process
        chartStream << "| " << currentTime << "   |    P" << processes[i].id << "    |\n";

        currentTime += processes[i].burstTime;

        // Record when CPU is taken back from the process
        chartStream << "| " << currentTime << "   |    ----    |\n";

        processes[i].turnaroundTime = currentTime - processes[i].arrivalTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    chartStream << "----------------------------------------\n";

    float avgWaitingTime = (float)totalWaitingTime / count;
    float avgTurnaroundTime = (float)totalTurnaroundTime / count;
    DisplayResult(avgWaitingTime, avgTurnaroundTime, "Priority Scheduling");

    // Show the Gantt chart in a message box
    MessageBox(NULL, chartStream.str().c_str(), "Gantt Chart for Priority Scheduling", MB_OK);
}

int GetInputValue(HWND hwndInput) {
    char inputBuffer[256];
    GetWindowText(hwndInput, inputBuffer, 256);
    return atoi(inputBuffer);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    static Scheduler scheduler;

    switch (Message) {

    case WM_CREATE:
    {
        // Create text input fields for arrival time, burst time, priority, and time quantum
        hwndArrivalInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 50, 100, 20, hwnd, (HMENU)101, NULL, NULL);
        CreateWindow("STATIC", "Arrival Time:", WS_CHILD | WS_VISIBLE,
            300, 50, 100, 20, hwnd, NULL, NULL, NULL);

        hwndBurstInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 100, 100, 20, hwnd, (HMENU)102, NULL, NULL);
        CreateWindow("STATIC", "Burst Time:", WS_CHILD | WS_VISIBLE,
            300, 100, 100, 20, hwnd, NULL, NULL, NULL);

        hwndPriorityInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 150, 100, 20, hwnd, (HMENU)103, NULL, NULL);
        CreateWindow("STATIC", "Priority:", WS_CHILD | WS_VISIBLE,
            300, 150, 100, 20, hwnd, NULL, NULL, NULL);

        hwndQuantumInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 400, 100, 20, hwnd, (HMENU)104, NULL, NULL);
        CreateWindow("STATIC", "Time Quantum:", WS_CHILD | WS_VISIBLE,
            300, 400, 100, 20, hwnd, NULL, NULL, NULL);

        // Create buttons for adding process and selecting scheduling algorithms
        CreateWindow("BUTTON", "Add Process", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 50, 150, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);


        /* Create text input fields for arrival time, burst time, priority, and time quantum
        hwndArrivalInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 50, 100, 20, hwnd, (HMENU)101, NULL, NULL);
        hwndBurstInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 100, 100, 20, hwnd, (HMENU)102, NULL, NULL);
        hwndPriorityInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 150, 100, 20, hwnd, (HMENU)103, NULL, NULL);
        hwndQuantumInput = CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
            400, 400, 100, 20, hwnd, (HMENU)104, NULL, NULL);*/


        // Create buttons for adding process and selecting scheduling algorithms
        CreateWindow("BUTTON", "Add Process", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 50, 150, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
        CreateWindow("BUTTON", "FCFS", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 100, 150, 30, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);
        CreateWindow("BUTTON", "Round Robin", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 150, 150, 30, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);
        CreateWindow("BUTTON", "SJN", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 200, 150, 30, hwnd, (HMENU)4, GetModuleHandle(NULL), NULL);
            CreateWindow("BUTTON", "SRJF", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
    50, 300, 150, 30, hwnd, (HMENU)7, GetModuleHandle(NULL), NULL);

        CreateWindow("BUTTON", "Priority Scheduling", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 250, 150, 30, hwnd, (HMENU)5, GetModuleHandle(NULL), NULL);
        CreateWindow("BUTTON", "Set Time Quantum", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 400, 150, 30, hwnd, (HMENU)6, GetModuleHandle(NULL), NULL);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case 1: // Add process

   {
    int arrivalTime = GetInputValue(hwndArrivalInput);
    int burstTime = GetInputValue(hwndBurstInput);
    int priority = GetInputValue(hwndPriorityInput);

    if (arrivalTime >= 0 && burstTime > 0 && priority >= 0) {
        scheduler.AddProcess(arrivalTime, burstTime, priority);
        // Display the entered values
        ostringstream messageStream;
        messageStream << "Process added with Arrival Time: " << arrivalTime
                      << ", Burst Time: " << burstTime
                      << ", Priority: " << priority;
        MessageBox(NULL, messageStream.str().c_str(), "Process Added", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(NULL, "Please enter valid values for all fields!", "Error", MB_OK | MB_ICONERROR);
    }
}
break;


        case 2: // Select FCFS
            scheduler.SimulateScheduling(&Scheduler::FCFS);
            break;
        case 3: // Select Round Robin
            scheduler.SimulateScheduling(&Scheduler::RoundRobin);
            break;
        case 4: // Select SJN
            scheduler.SimulateScheduling(&Scheduler::SJN);
            break;
        case 5: // Select Priority Scheduling
            scheduler.SimulateScheduling(&Scheduler::PriorityScheduling);
            break;


        case 6: // Set Time Quantum
            {
                int timeQuantum = GetInputValue(hwndQuantumInput);
                if (timeQuantum > 0) {
                    scheduler.SetTimeQuantum(timeQuantum);
                } else {
                    MessageBox(NULL, "Please enter a valid time quantum!", "Error", MB_OK | MB_ICONERROR);
                }
            }
            break;
            case 7: // Select SRJF
             scheduler.SimulateScheduling(&Scheduler::ShortestRemainingJobFirst);
            break;
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

// The 'main' function of Win32 GUI programs: this is where execution starts
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "myWindowClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "myWindowClass",
        "Process Scheduling Algorithms",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Main message loop:
    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


