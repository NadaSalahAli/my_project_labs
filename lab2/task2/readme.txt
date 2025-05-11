Matrix Multiplication with Per-Row Multithreading (Windows)
This program performs matrix multiplication using multithreading, where each thread computes one row of the result matrix. It is implemented in C and runs on Windows using _beginthreadex() and Windows-specific APIs.

🛠 Features
Multiplies two matrices stored in text files.

Each row of the result is computed by a separate thread.

Outputs execution time in microseconds.

Saves the result matrix in a separate file.

📁 File Format
Input Files (a.txt, b.txt)
Each matrix file should be formatted as:

makefile
Copy
Edit
row=2 col=3
1 2 3
4 5 6
Output File (c_per_row.txt)
The result is saved in the same format.

🚀 Compilation
Compile using a Windows-compatible C compiler (e.g., MSVC):

bash
Copy
Edit
cl /EHsc matrix_mult_per_row.c
Or use an IDE like Code::Blocks or Visual Studio.

🧪 Usage
bash
Copy
Edit
matrix_mult_per_row.exe [matrix_a_filename] [matrix_b_filename] [output_prefix]
Example
bash
Copy
Edit
matrix_mult_per_row.exe a b result
This reads from a.txt and b.txt, multiplies the matrices, and saves to result_per_row.txt.

📦 Functions Overview
read_matrix(filename) - Reads matrix from file.

write_matrix(filename, matrix) - Writes matrix to file.

compute_row(args) - Thread function to compute a single row.

multiply_matrices_per_row(a, b) - Manages threads and returns result matrix.

main() - Entry point, measures time and handles file I/O.

🧵 Multithreading Model
Each row of the resulting matrix is computed in a separate thread.

Threads are created using _beginthreadex() and synchronized with WaitForSingleObject().

🧹 Memory Management
All dynamically allocated memory (matrices and threads) is freed at the end of execution.

⛔ Error Handling
Checks for invalid file format or dimension mismatches.

Terminates with an error message if any file fails to open or matrix data is invalid.

Let me know if you'd like a version adapted for Linux or using column-wise threading
