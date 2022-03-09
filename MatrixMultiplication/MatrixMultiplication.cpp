#include <iostream>
#include <stdlib.h>
#include <time.h>

int* add(int* a, int*b, int n);
int* subtract(int* a, int* b, int n);
int* naiveMultiply(int* a, int* b, int n);
int* basicstrassenMultiply(int* a, int* b, int n);

int main()
{
    // seed the random function
    srand(time(NULL));

    int numCycles = 1000;

    double* cycles = new double[numCycles];
    for (int i = 0; i < numCycles; i++) cycles[i] = 0;
    int matrixSizeTotal = 0;

    for (int i = 0; i < numCycles; i++) {
        // create a random size for the matrices
        int size;
        size = (rand() % 100 + 100) / 2 * 2;

        // declaration and initialization of matrix A and matrix B
        int* a = new int[size * size];
        int* b = new int[size * size];
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                a[size * r + c] = rand() % 10 + 1;
            }
        }
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                b[size * r + c] = rand() % 10 + 1;
            }
        }

        // declaration of result matrix
        int* result = new int[size * size];

        // clock variables
        double T = 0.0;
        clock_t start, stop;

        // basic matrix multiplication
        start = clock();
        result = naiveMultiply(a, b, size);
        stop = clock();

        // calculate and display the clock cycles for basic matrix multiplication
        T = stop - start;
        std::cout << "Multiplying a " << size << " x " << size << " matrix by a " << size << " x " << size << " matrix took " << T << " clock cycles!" << std::endl;
        cycles[i] = T;

        // add the size of the matrix to the total
        matrixSizeTotal += size;
    }

    double totalCycles = 0;
    for (int i = 0; i < numCycles; i++) totalCycles += cycles[i];

    std::cout << "On average, multiplying " << numCycles << " " << matrixSizeTotal * 1.0 / numCycles << " x " << matrixSizeTotal * 1.0 / numCycles << " matrices takes " << totalCycles * 1.0 / numCycles << " cycles!" << std::endl;
}

int* add(int* a, int* b, int n) {
    int* result = new int[n * n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[n * i + j] = a[n * i + j] + b[n * i + j];
        }
    }
    return result;
}

int* subtract(int* a, int* b, int n) {
    int* result = new int[n * n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[n * i + j] = a[n * i + j] - b[n * i + j];
        }
    }
    return result;
}

int* naiveMultiply(int* a, int* b, int n) {
    int* result = new int[n * n];

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            result[n * r + c] = 0;
            for (int inner = 0; inner < n; inner++) {
                result[n * r + c] += a[n * r + inner] * b[n * inner + c];
            }
        }
    }

    return result;
}

int* basicstrassenMultiply(int* x, int* y, int n) {
    // base case, matrices of size 1 x 1
    if (n == 1) {
        // multiply x and y
        return naiveMultiply(x, y, n);
    }

    // size of all submatrices is half of large matrices
    int size = n / 2;
    int* a = new int[size * size];
    int* b = new int[size * size];
    int* c = new int[size * size];
    int* d = new int[size * size];
    int* e = new int[size * size];
    int* f = new int[size * size];
    int* g = new int[size * size];
    int* h = new int[size * size];
    // upper left subarray of x
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            a[size * i + j] = x[n * i + j];
        }
    }
    // upper right subarray of x
    for (int i = 0; i < size; i++) {
        for (int j = size + 1; j < n; j++) {
            b[size * i + (j - size + 1)] = x[n * i + j];
        }
    }
    // lower left subarray of x
    for (int i = size; i < n; i++) {
        for (int j = 0; j < size + 1; j++) {
            c[size * (i - size + 1) + j] = x[n * i + j];
        }
    }
    // lower right subarray of x
    for (int i = size + 1; i < n; i++) {
        for (int j = size + 1; j < n; j++) {
            d[size * (i - size + 1) + (j - size + 1)] = x[n * i + j];
        }
    }
    // upper left subarray of y
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            e[size * i + j] = y[n * i + j];
        }
    }
    // upper right subarray of y
    for (int i = 0; i < size; i++) {
        for (int j = size + 1; j < n; j++) {
            f[size * i + (j - size + 1)] = y[n * i + j];
        }
    }
    // lower left subarray of y
    for (int i = size + 1; i < n; i++) {
        for (int j = 0; j < size; j++) {
            g[size * (i - size + 1) + j] = y[n * i + j];
        }
    }
    // lower right subarray of y
    for (int i = size + 1; i < n; i++) {
        for (int j = size + 1; j < n; j++) {
            h[size * (i - size + 1) + (j - size + 1)] = y[n * i + j];
        }
    }

    // calculating submatrices
    int* p1 = basicstrassenMultiply(a, subtract(f, h, size), size);
    int* p2 = basicstrassenMultiply(add(a, b, size), h, size);
    int* p3 = basicstrassenMultiply(add(c, d, size), e, size);
    int* p4 = basicstrassenMultiply(d, subtract(g, e, size), size);
    int* p5 = basicstrassenMultiply(add(a, d, size), add(e, h, size), size);
    int* p6 = basicstrassenMultiply(subtract(b, d, size), add(g, h, size), size);
    int* p7 = basicstrassenMultiply(subtract(a, c, size), add(e, f, size), size);

    // calculating submatrices
    int* c11 = add(subtract(add(p5, p4, size), p2, size), p6, size);
    int* c12 = add(p1, p2, size);
    int* c21 = add(p3, p4, size);
    int* c22 = subtract(subtract(add(p1, p5, size), p3, size), p7, size);

    // generate result matrix from "c" matrices
    int* result = new int[n * n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // upper right
            if (i < size && j < size) {
                result[n * i + j] = c11[size * i + j];
            }
            // upper left
            else if (i < size && j > size) {
                result[n * i + j] = c12[size * i + (j - size)];
            }
            // lower right
            else if (i > size && j < size) {
                result[n * i + j] = c21[size * (i - size) + j];
            }
            // lower left
            else {
                result[n * i + j] = c22[size * (i - size) + (j - size)];
            }
        }
    }

    return result;
}