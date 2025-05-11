Matrix Multiplication (Per Matrix Method)
This project implements matrix multiplication in C using a simple single-threaded per-matrix approach. The matrices are read from files, multiplied, and the result is saved in a new output file. The execution time is measured using Windows high-resolution performance counters.

🧮 Features
Reads two matrices from files.

Performs matrix multiplication.

Writes the resulting matrix to an output file.

Measures and displays the execution time.

Supports customizable input and output filenames via command-line arguments.

📂 File Format
Each input file must follow this format:

makefile
Copy
Edit
row=2 col=3
1 2 3
4 5 6
Where:

row and col specify the dimensions of the matrix.

The rest of the file contains the matrix data, row by row.

🚀 Usage
🔧 Compile
bash
Copy
Edit
gcc matrix_per_matrix.c -o matrix_per_matrix
Ensure you are compiling on a Windows environment due to the use of windows.h.

▶️ Run
bash
Copy
Edit
./matrix_per_matrix [matrix1_filename] [matrix2_filename] [output_prefix]
matrix1_filename: (optional) name of the first matrix file (default: a)

matrix2_filename: (optional) name of the second matrix file (default: b)

output_prefix: (optional) prefix for the output file (default: c)

📄 Example
Given files:

a.txt

b.txt

Run:

bash
Copy
Edit
./matrix_per_matrix
This generates a file named c_per_matrix.txt with the result.

🧼 Memory Management
All dynamically allocated memory for matrices is properly freed at the end of execution.

📊 Performance
The execution time is measured using QueryPerformanceCounter and displayed in seconds.

Example Output:

sql
Copy
Edit
Method: Per Matrix  
Threads created: 1  
Time taken: 0.000003 seconds  
📌 Notes
Only supports integer matrix elements.

The matrix multiplication is valid only if the number of columns in the first matrix equals the number of rows in the second.

Output matrix will be of size: a.rows x b.cols.
