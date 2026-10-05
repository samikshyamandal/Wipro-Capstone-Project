Wipro Capstone Project - Industrial Conveyor Motor Monitor
By Samikshya Mandal

This is my final capstone project. It includes a custom Linux device driver and a C++ dashboard to monitor an industrial conveyor belt.

What the project does:
- Linux Kernel Driver: Simulates a motor's RPM and temperature at the kernel level.
- C++ Dashboard: Reads the real-time data from the driver and displays it in the terminal.
- Fault Testing: You can manually trigger an overheat or a belt jam state to see how the system reacts.

Files in this repository:
- conveyor_driver.c : The main Linux kernel module code
- Makefile : Compiles the kernel module
- dashboard.cpp : The C++ program for the display
- setup.sh : A bash script that builds the code, loads the driver, and starts the dashboard
