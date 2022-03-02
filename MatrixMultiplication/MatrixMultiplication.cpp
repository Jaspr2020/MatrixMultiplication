#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
    // seed the random function
    srand(time(NULL));

    // create a random size for the matrices
    int ran;
    ran = rand() % 3 + 2;


    // dimensions of matrix A and matrix B
    int aRows = ran;
    int aCols = ran;
    int bRows = ran;
    int bCols = ran;

    // declaration and initialization of matrix A and matrix B
    int *a = new int[aRows * aCols];
    int *b = new int[bRows * bCols];
    for (int r = 0; r < aRows; r++) {
        for (int c = 0; c < aCols; c++) {
            a[aRows * r + c] = rand() % 10 + 1;
        }
    }
    for (int r = 0; r < bRows; r++) {
        for (int c = 0; c < bCols; c++) {
            b[bRows * r + c] = rand() % 10 + 1;
        }
    }

    // declaration of result matrix
    int *result = new int[aRows * bCols];


    // basic matrix multiplication
    for (int r = 0; r < aRows; r++) {
        for (int c = 0; c < bCols; c++) {
            result[aRows * r + c] = 0;
            for (int inner = 0; inner < bRows; inner++) {
                result[aRows * r + c] += a[aRows * r + inner] * b[bRows * inner + c];
            }
        }
    }

    // display matrix A
    for (int r = 0; r < aRows; r++) {
        for (int c = 0; c < aCols; c++) {
            std::cout << a[aRows * r + c] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // display matrix B
    for (int r = 0; r < bRows; r++) {
        for (int c = 0; c < bCols; c++) {
            std::cout << b[bRows * r + c] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // display result matrix
    for (int r = 0; r < aRows; r++) {
        for (int c = 0; c < bCols; c++) {
            std::cout << result[aRows * r + c] << " ";
        }
        std::cout << std::endl;
    }
}
