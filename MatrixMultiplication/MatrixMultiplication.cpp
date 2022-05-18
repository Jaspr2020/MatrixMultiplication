#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <chrono>

const int k = 4096;
SIZE_T memoryUsage;
std::ofstream fout;

struct data
{
    double time;
    SIZE_T memory;

    data()
    {
        time = 0;
        memory = 0;
    }
    data(double t, SIZE_T m)
    {
        time = t;
        memory = m;
    }
};

std::vector<int> add(int a(int index), int b(int index), int n);
std::vector<int> subtract(int a(int index), int b(int index), int n);
std::vector<int> naiveMultiply(std::vector<int> a, std::vector<int> b, int n);
std::vector<int> StrassenMultiply(std::vector<int> a, std::vector<int> b, int n);
data test(int size, int cycles, std::vector<int> multiplyFunc(std::vector<int> a, std::vector<int> b, int n));



int main()
{
    fout.open("D:\\Strassen\\BAM.txt");
    srand(time(NULL));
    int cycles = 10;
    double timeTotal = 0;
    SIZE_T sizeTotal = 0;
    data runs[10];
    for (int i = 0; i <= 4096; i++)
    {
        timeTotal = 0;
        sizeTotal = 0;
        //k = 1;
        //std::cout << "2^" << i << " Square Matrix:\n";
        std::cout << i << "\t";
        //fout << "2^" << i << "\n";

        //for (int j = 0; j <= i; k = pow(2,++j))
        //{
            //std::cout << "\tStrassen k="<< j << ":\n";
            //fout << "\tk=2^" << j << "\tTime\tHeap\tIO\n";
        fout << i << "\t";
        for (int j = 0; j < 10; j++)
        {
            runs[j] = test(/*pow(2, i)*/i, cycles, StrassenMultiply); //k=2^i is equivalent to naive
            timeTotal += runs[j].time;
            sizeTotal += runs[j].memory;
        }
        double timeAvg = timeTotal / cycles;
        SIZE_T memAvg = sizeTotal / cycles;
        double timeStdDev = 0;
        SIZE_T memStdDev = 0;
        for (int j = 0; j < 10; j++)
        {
            timeStdDev += pow(runs[j].time - timeAvg, 2);
            memStdDev += pow(runs[j].memory - memAvg, 2);
        }
        timeStdDev = sqrt(timeStdDev/cycles);
        memStdDev = sqrt(memStdDev/cycles);
            
        //}
        fout << timeAvg << "\t" << timeStdDev << "\t" << memAvg << "\t" << memStdDev << "\n";
        std::cout << timeAvg << "\t" << timeStdDev << "\t" << memAvg << "\t" << memStdDev << "\n";
    }
    fout.close();
    system("pause");
}

data test(int size, int cycles, std::vector<int> multiplyFunc(std::vector<int> a, std::vector<int> b, int n))
{
    //int size = pow(2, power);
    std::vector<int> a(size * size);
    std::vector<int> b(size * size);
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

    for (int i = 0; i < cycles; i++) {
        memoryUsage = 0;
        // declaration and initialization of matrix A and matrix B
        

        // declaration of result matrix
        std::vector<int> result(size*size);

        // matrix multiplication
        auto start = std::chrono::steady_clock::now();
        result = multiplyFunc(a, b, size);
        auto stop = std::chrono::steady_clock::now();
        std::chrono::duration<double> T = (stop - start);

        //std::cout << "\t\tTime: " << T.count() << "s, Heap Usage:" << memoryUsage << "B, IO Usage:" << a.size() + b.size() + result.size() << "B\n";
        //fout << "\t\t" << T.count() << "\t" << memoryUsage << "\t" << a.size() + b.size() + result.size() << "\n";
        //fout << T.count() << "\t" << memoryUsage << "\t" << a.size() + b.size() + result.size() << "\n";
        return data(T.count(), memoryUsage + a.size() + b.size() + result.size());
    }
}

std::vector<int> add(std::vector<int> a, std::vector<int> b, int n) {
    std::vector<int> result(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[n * i + j] = a[n * i + j] + b[n * i + j];
        }
    }
    return result;
}

std::vector<int> subtract(std::vector<int> a, std::vector<int> b, int n) {
    std::vector<int> result(n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[n * i + j] = a[n * i + j] - b[n * i + j];
        }
    }
    return result;
}

std::vector<int> naiveMultiply(std::vector<int> a, std::vector<int> b, int n) {
    std::vector<int> result(n * n);

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

//class Matrix
//{
//public:
//    Matrix(std::vector<int>* vector)
//    {
//        realMatrix = vector;
//    }
//    int at(int index)
//    {
//        return realMatrix->at(index);
//    }
//private:
//    std::vector<int> *realMatrix;
//};
//
//class ULMatrix : Matrix
//{
//    int at(int index)
//    {
//
//    }
//};
//
//class URMatrix : Matrix
//{
//
//};
//
//class BLMatrix : Matrix
//{
//
//};
//
//class BRMatrix : Matrix
//{
//
//};



std::vector<int> StrassenMultiply(std::vector<int> x, std::vector<int> y, int n) {
    // base case
    if (n <= k) {
        // multiply x and y
        return naiveMultiply(x, y, n);
    }
    //Each subarray is a quadrant, having 1/2 the width and height
    if (n % 2) //pad arrays if odd
    {
        std::vector<int> oldX = x;
        std::vector<int> oldY = y;
        x = std::vector<int>((n+1) * (n+1), 0);
        y = std::vector<int>((n+1) * (n+1), 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                x[i * (n+1) + j] = oldX[i * n + j];
                y[i * (n+1) + j] = oldY[i * n + j];
            }
        n++;
    }
    int size = n / 2; //because of int division, will round correctly
    int arrLen = size * size;

    std::vector<int> a(arrLen);   // upper left subarray of x
    std::vector<int> b(arrLen);   // upper right subarray of x
    std::vector<int> c(arrLen);   // lower left subarray of x
    std::vector<int> d(arrLen);   // lower right subarray of x
    std::vector<int> e(arrLen);   // upper left subarray of y
    std::vector<int> f(arrLen);   // upper right subarray of y
    std::vector<int> g(arrLen);   // lower left subarray of y
    std::vector<int> h(arrLen);   // lower right subarray of y

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            a[i * size + j] = x[i * n + j];
            b[i * size + j] = x[i * n + j + size];
            c[i * size + j] = x[(i + size) * n + j];
            d[i * size + j] = x[(i + size) * n + j + size];
            e[i * size + j] = y[i * n + j];
            f[i * size + j] = y[i * n + j + size];
            g[i * size + j] = y[(i + size) * n + j];
            h[i * size + j] = y[(i + size) * n + j + size];
        }

    // calculating submatrices
    std::vector<int> fsubh = subtract(f, h, size);
    std::vector<int> aaddb = add(a, b, size);
    std::vector<int> caddd = add(c, d, size);
    std::vector<int> gsube = subtract(g, e, size);
    std::vector<int> aaddd = add(a, d, size);
    std::vector<int> eaddh = add(e, h, size);
    std::vector<int> bsubd = subtract(b, d, size);
    std::vector<int> gaddh = add(g, h, size);
    std::vector<int> asubc = subtract(a, c, size);
    std::vector<int> eaddf = add(e, f, size);

    std::vector<int> p1 = StrassenMultiply(a, fsubh, size);
    std::vector<int> p2 = StrassenMultiply(aaddb, h, size);
    std::vector<int> p3 = StrassenMultiply(caddd, e, size);
    std::vector<int> p4 = StrassenMultiply(d, gsube, size);
    std::vector<int> p5 = StrassenMultiply(aaddd, eaddh, size);
    std::vector<int> p6 = StrassenMultiply(bsubd, gaddh, size);
    std::vector<int> p7 = StrassenMultiply(asubc, eaddf, size);



    // calculating submatrices
    std::vector<int> c11InnerAdd = add(p5, p4, size);
    std::vector<int> c11InnerSub = subtract(c11InnerAdd, p2, size);
    std::vector<int> c11 = add(c11InnerSub, p6, size);
    std::vector<int> c12 = add(p1, p2, size);
    std::vector<int> c21 = add(p3, p4, size);
    std::vector<int> c22InnerAdd = add(p1, p5, size);
    std::vector<int> c22InnerSub = subtract(c22InnerAdd, p3, size);
    std::vector<int> c22 = subtract(c22InnerSub, p7, size);

    // generate result matrix from "c" matrices
    std::vector<int> result(n * n);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            result[i * n + j] = c11[i * size + j];
            result[i * n + j + size] = c12[i * size + j];
            result[(i + size) * n + j] = c21[i * size + j];
            result[(i + size) * n + j + size] = c22[i * size + j];
        }

    //Calculate Heap Usage
    memoryUsage += a.size();
    memoryUsage += b.size();
    memoryUsage += c.size();
    memoryUsage += d.size();
    memoryUsage += e.size();
    memoryUsage += f.size();
    memoryUsage += g.size();
    memoryUsage += h.size();
    memoryUsage += fsubh.size();
    memoryUsage += aaddb.size();
    memoryUsage += caddd.size();
    memoryUsage += gsube.size();
    memoryUsage += aaddd.size();
    memoryUsage += eaddh.size();
    memoryUsage += bsubd.size();
    memoryUsage += gaddh.size();
    memoryUsage += asubc.size();
    memoryUsage += eaddf.size();
    memoryUsage += p1.size();
    memoryUsage += p2.size();
    memoryUsage += p3.size();
    memoryUsage += p4.size();
    memoryUsage += p5.size();
    memoryUsage += p6.size();
    memoryUsage += p7.size();
    memoryUsage += c11.size();
    memoryUsage += c11InnerAdd.size();
    memoryUsage += c11InnerSub.size();
    memoryUsage += c12.size();
    memoryUsage += c21.size();
    memoryUsage += c22InnerAdd.size();
    memoryUsage += c22InnerSub.size();
    memoryUsage += c22.size();

    return result;
}