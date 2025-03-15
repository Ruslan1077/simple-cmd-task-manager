# Windows Task Monitor (Console-based)
A lightweight console-based task manager for Windows, similar to `htop`.

## Features
✅ Display system information:
   - CPU usage
   - RAM usage
   - System uptime  
✅ ==(SOON)== Show a real-time list of running processes:
   - PID (Process ID)
   - Process name
   - CPU and memory usage per process
   - Process state (Running, Suspended, Not Responding)
   - User who started the process  
✅ ==(SOON)== Auto-refresh system data every N seconds
✅ ==(SOON)== (Optional) Interactive process management:
   - Kill process by PID
   - Change process priority  

## Installation  
### Prerequisites  
- Windows 10  
- C++20
- Microsoft Visual Studio 22 

### Clone the repository
```sh
git clone https://github.com/your-username/windows-task-monitor.git
cd windows-task-monitor
```

## Usage
Run the compiled executable in the terminal:
```sh
task_monitor.exe
```

### Example Output:
```
System Uptime: 12:34:56
CPU Usage: 24.5%
RAM Usage: 8.2 GB / 32 GB

PID    USER        CPU%   MEM%   STATUS       COMMAND
1234   SYSTEM      3.2    1.1    Running      svchost.exe
5678   user        12.5   3.0    Running      chrome.exe
91011  user        8.9    2.5    Suspended    notepad.exe
```

## Technical Details
This project retrieves system and process information using:
- ==soon==

## Future Improvements
- ==soon==

## License
This project is licensed under the MIT License.
