#include <iostream>
#include <stdlib.h>
#include <time.h>

int* add(int* a, int*b, int n);
int* subtract(int* a, int* b, int n);
int* naiveMultiply(int* a, int* b, int n);
int* basicStrassenMultiply(int* a, int* b, int n);
double test(int power, int cycles, int* multiplyFunc(int* a, int* b, int n));

int main()
{
    srand(time(NULL));
    int cycles = 10;
    double timing;
    for (int i = 1; i <= 15; i++) //2^31 is the max int so 2^15 x 2^15 is the largest 2^n square matrix
    {
        timing = test(i, cycles, naiveMultiply);
        std::cout << "Multiplying 2^" << i << " square matrices with naive multiplication takes " << timing << " cycles" << std::endl;
        timing = test(i, cycles, basicStrassenMultiply);
        std::cout << "Multiplying 2^" << i << " square matrices with Strassen's Algorithm takes " << timing << " cycles" << std::endl;
    }

    system("pause");
}

double test(int power, int cycles, int* multiplyFunc(int* a, int* b, int n))
{
    int size = pow(2, power);
    double totalTime = 0.0;

    for (int i = 0; i < cycles; i++) {
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
        int* result;

        // clock variables
        double T = 0.0;
        clock_t start, stop;

        // matrix multiplication
        start = clock();
        result = multiplyFunc(a, b, size);
        stop = clock();
        T = stop - start;
        totalTime += T;
        
        delete[] a;
        delete[] b;
        delete[] result;
    }

    return totalTime / (double)cycles;
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

int* basicStrassenMultiply(int* x, int* y, int n) {
    // base case, matrices of size 1 x 1
    if (n == 1) {
        // multiply x and y
        return naiveMultiply(x, y, n);
    }
    //Each subarray is a quadrant, having 1/2 the width and height
    int size = n / 2;
    int arrLen = size * size;

    int* a = new int[arrLen];   // upper left subarray of x
    int* b = new int[arrLen];   // upper right subarray of x
    int* c = new int[arrLen];   // lower left subarray of x
    int* d = new int[arrLen];   // lower right subarray of x
    int* e = new int[arrLen];   // upper left subarray of y
    int* f = new int[arrLen];   // upper right subarray of y
    int* g = new int[arrLen];   // lower left subarray of y
    int* h = new int[arrLen];   // lower right subarray of y
    
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            a[i*size + j] = x[i*n + j];
            b[i*size + j] = x[i*n + j+size];
            c[i*size + j] = x[(i+size)*n + j];
            d[i*size + j] = x[(i+size)*n + j+size];
            e[i*size + j] = y[i*n + j];
            f[i*size + j] = y[i*n + j+size];
            g[i*size + j] = y[(i+size)*n + j];
            h[i*size + j] = y[(i+size)*n + j+size];
        }

    // calculating submatrices
    int* fsubh = subtract(f, h, size);
    int* aaddb = add(a, b, size);
    int* caddd = add(c, d, size);
    int* gsube = subtract(g, e, size);
    int* aaddd = add(a, d, size);
    int* eaddh = add(e, h, size);
    int* bsubd = subtract(b, d, size);
    int* gaddh = add(g, h, size);
    int* asubc = subtract(a, c, size);
    int* eaddf = add(e, f, size);

    int* p1 = basicStrassenMultiply(a, fsubh, size);
    int* p2 = basicStrassenMultiply(aaddb, h, size);
    int* p3 = basicStrassenMultiply(caddd, e, size);
    int* p4 = basicStrassenMultiply(d, gsube, size);
    int* p5 = basicStrassenMultiply(aaddd, eaddh, size);
    int* p6 = basicStrassenMultiply(bsubd, gaddh, size);
    int* p7 = basicStrassenMultiply(asubc, eaddf, size);

    delete[] fsubh;
    delete[] aaddb;
    delete[] caddd;
    delete[] gsube;
    delete[] aaddd;
    delete[] eaddh;
    delete[] bsubd;
    delete[] gaddh;
    delete[] asubc;
    delete[] eaddf;

    // calculating submatrices
    int* c11InnerAdd = add(p5, p4, size);
    int* c11InnerSub = subtract(c11InnerAdd, p2, size);
    int* c11 = add(c11InnerSub, p6, size);
    int* c12 = add(p1, p2, size);
    int* c21 = add(p3, p4, size);
    int* c22InnerAdd = add(p1, p5, size);
    int* c22InnerSub = subtract(c22InnerAdd, p3, size);
    int* c22 = subtract(c22InnerSub, p7, size);

    delete[] c11InnerAdd;
    delete[] c11InnerSub;
    delete[] c22InnerAdd;
    delete[] c22InnerSub;

    // generate result matrix from "c" matrices
    int* result = new int[n * n];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            result[i*n + j] = c11[i*size + j];
            result[i*n + j+size] = c12[i*size + j];
            result[(i+size)*n + j] = c21[i*size + j];
            result[(i+size)*n + j + size] = c22[i*size + j];
        }

    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
    delete[] e;
    delete[] f;
    delete[] g;
    delete[] h;
    delete[] p1;
    delete[] p2;
    delete[] p3;
    delete[] p4;
    delete[] p5;
    delete[] p6;
    delete[] p7;
    delete[] c11;
    delete[] c12;
    delete[] c21;
    delete[] c22;

    return result;
}