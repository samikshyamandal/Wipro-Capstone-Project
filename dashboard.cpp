#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <chrono>

using namespace std;

void printDashboard(int rpm, int temp, int state) {
    system("clear");
    cout << "=========================================\n";
    cout << "  INDUSTRIAL CONVEYOR TELEMETRY ENGINE   \n";
    cout << "=========================================\n\n";
    
    cout << "  Motor RPM   : " << rpm << (rpm < 50 ? " [CRITICAL]" : " [OK]") << "\n";
    cout << "  Temperature : " << temp << " C" << (temp > 90 ? " [WARNING]" : " [OK]") << "\n\n";
    
    cout << "  System State: ";
    if (state == 0) cout << "\033[1;32mNORMAL OPERATION\033[0m\n";
    else if (state == 1) cout << "\033[1;33mFAULT: OVERHEATING\033[0m\n";
    else if (state == 2) cout << "\033[1;31mFAULT: BELT JAMMED\033[0m\n";
    
    cout << "\n=========================================\n";
    cout << "Controls (Type and press Enter):\n";
    cout << "[0] Reset to Normal  [1] Inject Overheat  [2] Inject Belt Jam\n";
    cout << "=========================================\n";
}

void telemetry_loop() {
    char read_buf[256];
    while (true) {
        int fd = open("/dev/conveyor_motor", O_RDONLY);
        if (fd < 0) {
            cerr << "Error: Cannot open device driver. Is the module loaded?\n";
            exit(1);
        }
        
        int bytes = read(fd, read_buf, sizeof(read_buf));
        close(fd);
        
        if (bytes > 0) {
            read_buf[bytes] = '\0';
            int rpm, temp, state;
            sscanf(read_buf, "%d %d %d", &rpm, &temp, &state);
            printDashboard(rpm, temp, state);
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

int main() {
    thread t_loop(telemetry_loop);
    
    string command;
    while (cin >> command) {
        if (command == "0" || command == "1" || command == "2") {
            int fd = open("/dev/conveyor_motor", O_WRONLY);
            if (fd >= 0) {
                write(fd, command.c_str(), 1);
                close(fd);
            }
        }
    }
    t_loop.join();
    return 0;
}
