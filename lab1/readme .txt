🐚 Custom Windows Shell in C
This is a simple command-line shell implemented in C for Windows. It supports basic built-in commands (cd, echo, export), background process execution, variable expansion (like $VAR), and logging to a file (shell.log).

🛠 Features
Custom shell prompt: myshell>

Built-in commands:

cd [directory]: Change the current working directory

echo [args]: Print arguments to the console

export VAR=value: Set environment variables

Variable expansion: Use $VAR to expand environment variables

Background execution: Use & at the end of a command to run in the background

External command execution via CreateProcess

Logging of shell start and exit events to shell.log

📁 Project Structure
main.c: The core implementation of the shell

shell.log: Log file generated at runtime (created automatically)

🧪 Example Usage
shell
Copy
Edit
myshell> echo "Hello World"
Hello World

myshell> export PATH="C:\\MyPath"
myshell> echo $PATH
C:\MyPath

myshell> cd C:\Windows
myshell> notepad &     # Run Notepad in the background
📌 Compilation
This project is designed for Windows and uses Windows-specific APIs like CreateProcess and _chdir.

🔧 Requirements
A C compiler that supports Windows headers (e.g., GCC via MinGW or MSVC)

Windows OS

🧱 Build with GCC (MinGW)
sh
Copy
Edit
gcc main.c -o myshell.exe
🚀 Run the Shell
sh
Copy
Edit
myshell.exe
📝 Notes
Built-in command handling avoids CreateProcess for speed and correctness.

Variable expansion supports both user-defined (export) and system (getenv) variables.

Logs are appended to shell.log in the current directory.

🔒 Limitations
No command history

No piping or redirection

No autocomplete

Only supports a limited set of built-in commands


