Matrix Multiplication using Threads (Per Row & Per Element)
Overview
This project implements two versions of multithreaded matrix multiplication in C:

Per Row Threading (Windows using _beginthreadex)

Per Element Threading (POSIX using pthread_create)

Both versions read matrices from files, perform matrix multiplication using threads for parallelism, and write the result to an output file while measuring execution time.

File Structure
less
Copy
Edit
matrix_per_row.c        // Windows: Thread-per-row multiplication
matrix_per_element.c    // Linux/Mac: Thread-per-element multiplication
a.txt                   // Example input matrix A
b.txt                   // Example input matrix B
Matrix File Format
Each input matrix file must be formatted as:

python-repl
Copy
Edit
row=R col=C
a11 a12 a13 ...
a21 a22 a23 ...
...
First line specifies the number of rows and columns.

Each subsequent line contains one row of the matrix.

Example (a.txt):

makefile
Copy
Edit
row=2 col=3
1 2 3
4 5 6
Compilation
Per Row (Windows Only)
bash
Copy
Edit
gcc matrix_per_row.c -o matrix_per_row.exe
Uses <windows.h> and <process.h>. Only works on Windows.

Per Element (Linux/Mac)
bash
Copy
Edit
gcc matrix_per_element.c -o matrix_per_element -lpthread
Uses pthread.h. Works on Linux and macOS.

Execution
bash
Copy
Edit
./matrix_per_row a b c       # Output: c_per_row.txt
./matrix_per_element a b c   # Output: c_per_element.txt
a and b are input matrix filenames (without .txt).

c is the output file prefix.

Features
✅ Per Row Multiplication
One thread per row of the result matrix.

Efficient for large matrices where thread count remains low.

Timed using QueryPerformanceCounter.

✅ Per Element Multiplication
One thread per element in the result matrix.

Fine-grained parallelism; better for small to medium matrices.

Timed using gettimeofday.

Output
Each run prints:

Method used (Per Row or Per Element)

Number of threads created

Execution time (microseconds or seconds)

Example Output
sql
Copy
Edit
Method: Per Element
Threads created: 6
Time taken: 0.000456 seconds (456 microseconds)
Notes
Ensure matrix dimensions are compatible: A.cols == B.rows.

Memory is dynamically allocated and freed properly.

The per-element version may create a large number of threads — use with caution on large matrices.
