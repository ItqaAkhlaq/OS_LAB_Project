
<h2>Process Scheduling Simulator</h2>
<h3>Overview</h3>
The Process Scheduling Simulator is a Windows-based graphical application in C++ designed to simulate various process scheduling algorithms. Users can input process details, select a scheduling algorithm, and view simulation results, including Gantt charts, average waiting times, and turnaround times.

<h2>Group Members</h2>
<ul>
    <li>Roll No#S095: Jawad Ahmed</li>
    <li>Roll No#R024: Itqa Akhlaq</li>
    <li>Roll No#R002: Amna Zafar</li>
</ul>

<h2>Program:</h2>
BSIT 4th Regular

<h2>Features</h2>
<ol>
    <li>Add Processes:
        <ul>
            <li>Users can input process details, including arrival time, burst time, and priority.</li>
        </ul>
    </li>
    <li>Set Time Quantum (for Round Robin):
        <ul>
            <li>Specify the time quantum for the Round Robin scheduling algorithm.</li>
        </ul>
    </li>
    <li>Simulates following Scheduling Algorithms:
        <ul>
            <li>First-Come, First-Served (FCFS)</li>
            <li>Round Robin (RR)</li>
            <li>Shortest Job Next (SJN)</li>
            <li>Shortest Remaining Job First (SRJF)</li>
            <li>Priority Scheduling</li>
        </ul>
    </li>
    <li>Visualize Gantt Charts:
        <ul>
            <li>Display the scheduling process using Gantt charts.</li>
        </ul>
    </li>
    <li>View Statistics:
        <ul>
            <li>Show average waiting time and turnaround time for the processes.</li>
        </ul>
    </li>
</ol>

<h2>Prerequisites</h2>
<ul>
    <li>Windows Operating System:
        <ul>
            <li>The application runs on Windows and requires a Windows environment.</li>
        </ul>
    </li>
    <li>C++ Compiler:
        <ul>
            <li>Use a C++ compiler with Win32 API support (e.g., MinGW, MSVC, GNU Compiler).</li>
            <li>If you don't have the gdi32 library in your project file, add it.</li>
        </ul>
    </li>
</ul>

<h2>Building the Project</h2>
<ol>
    <li>Clone the Repository</li>
    <li>git clone https://github.com/ItqaAkhlaq/OS_LAB_Project.git</li>
    <li>Navigate to the Project Directory</li>
    <li>cd process-scheduling-simulator</li>
    <li>Compile the Code</li>
    <li>g++ -o ProcessScheduler main.cpp -mwindows (Replace g++ with your compiler if different.)</li>
    <li>Run the Executable</li>
    <li>ProcessScheduler.exe</li>
</ol>

<h2>Usage</h2>
<ol>
    <li>Launch the Application:
        <ul>
            <li>Double-click ProcessScheduler.exe or run it from the command line.</li>
        </ul>
    </li>
    <li>Add Processes:
        <ul>
            <li>Enter arrival time, burst time, and priority.</li>
            <li>Click “Add Process” to add a process to the scheduler.</li>
        </ul>
    </li>
    <li>Set Time Quantum (for Round Robin):
        <ul>
            <li>Specify the time quantum.</li>
        </ul>
    </li>
    <li>Simulate a Scheduling Algorithm:
        <ul>
            <li>Click the button for the desired scheduling algorithm (e.g., "FCFS", "Round Robin").</li>
            <li>View the results, including Gantt charts and statistics, in the message boxes.</li>
        </ul>
    </li>
</ol>

<h2>Input Validation</h2>
<ul>
    <li>Ensure all input fields contain valid positive integers.</li>
    <li>Check that at least one process is added before starting the simulation.</li>
</ul>

<h2>Code Structure</h2>
<ul>
    <li>main.cpp: Contains the entire application code, including the WinMain function, WndProc function, Process structure, and Scheduler class with various scheduling algorithms.</li>
</ul>

<h2>Future Improvements</h2>
<ul>
    <li>Enhanced input validation and error handling.</li>
    <li>Improved</li></ul>
