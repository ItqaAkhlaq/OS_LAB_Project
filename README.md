
<h2> Process Scheduling Simulator </h2><br>
Overview <br>
The Process Scheduling Simulator is a Windows-based graphical application in C++ designed to simulate various process scheduling algorithms. Users can input process details, select a scheduling algorithm, and view simulation results, including Gantt charts, average waiting times, and turnaround times.
Group Members <br>
•	Roll No#S095: Jawad Ahmed <br>
•	Roll No#R024: Itqa Akhlaq<br>
•	Roll No#R002: Amna Zafar<br>
Program<br>
BSIT 4th Regular <br>
<h2>Features </h2><br>
1.	Add Processes: <br>
o	Users can input process details, including arrival time, burst time, and priority. <br>
2.	Set Time Quantum (for Round Robin): <br>
o	Specify the time quantum for the Round Robin scheduling algorithm. <br>
3.	Simulate Scheduling Algorithms: <br>
o	Supports the following algorithms: <br>
	First-Come, First-Served (FCFS)<br>
	Round Robin (RR)<br>
	Shortest Job Next (SJN) <br>
	Shortest Remaining Job First (SRJF)<br>
	Priority Scheduling<br>
4.	Visualize Gantt Charts:<br> 
o	Display the scheduling process using Gantt charts.<br>
5.	View Statistics: <br>
o	Show average waiting time and turnaround time for the processes.<br>
<h2>Prerequisites </h2><br>
1.	Windows Operating System: <br>
o	The application runs on Windows and requires a Windows environment.<br>
2.	C++ Compiler: <br>
o	Use a C++ compiler with Win32 API support (e.g., MinGW, MSVC, GNU Compiler). <br> If you don't gdi32 library in your project file add it. <br>
<h2>Building the Project </h2> <br>
1.	Clone the Repository: <br>
2.	git clone https://github.com/ItqaAkhlaq/OS_LAB_Project.git <br>
3.	Navigate to the Project Directory: <br>
4.	cd process-scheduling-simulator <br>
5.	Compile the Code: <br>
6.	g++ -o ProcessScheduler main.cpp -mwindows<br>
o	Replace g++ with your compiler if different.<br>
7.	Run the Executable:<br>
8.	ProcessScheduler.exe<br>
<h2>Usage  </h2> <br>
1.	Launch the Application: <br>
o	Double-click ProcessScheduler.exe or run it from the command line. <br>
2.	Add Processes: <br>
o	Enter arrival time, burst time, and priority. <br>
o	Click “Add Process” to add a process to the scheduler. <br>
3.	Set Time Quantum (for Round Robin): <br>
4.Specify the time quantum. Simulate a Scheduling Algorithm:<br>
o	Click the button for the desired scheduling algorithm (e.g., "FCFS", "Round Robin").<br>
o	View the results, including Gantt charts and statistics, in the message boxes.<br>
<h2>Input Validation </h2><br>
•	Ensure all input fields contain valid positive integers. <br>
•	Check that at least one process is added before starting the simulation. <br>
<h2>Code Structure </h2><br>
•	main.cpp: Contains the entire application code, including the WinMain function, WndProc function, Process structure, and Scheduler class with various scheduling algorithms.<br>
<h2>Future Improvements </h2> <br>
•	Enhanced input validation and error handling.<br>
•	Improved visual presentation of Gantt charts. <br>
•	Separation of GUI code and scheduling logic for better maintainability. <br>
•	Additional scheduling algorithms as needed. <br>
<h2>Contributing </h2><br>
1.	Fork the repository. <br>
2.	Create your feature branch (git checkout -b feature/YourFeature). <br>
3.	Commit your changes (git commit -m 'Add some feature'). <br>
4.	Push to the branch (git push origin feature/YourFeature). <br>
5.	Open a pull request. <br>
Acknowledgements <br>
•	Inspiration and guidance from various online resources on process scheduling algorithms and Win32 API programming. <br>

