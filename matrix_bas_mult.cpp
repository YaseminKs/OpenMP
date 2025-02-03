#include <iostream>
#include <cstdlib>
#include <omp.h>

#define N 16  // Matrix size (N x N)

// Function to multiply matrices using OpenMP
void matrixMultiplicationOMP( double* A, double* B, double* C, int size ){
    #pragma omp parallel for collapse(2)
    for( int i = 0 ; i < size ; i++ ){
        for( int j = 0 ; j < size ; j++ ){
            double sum = 0.0;
            for( int k = 0 ; k < size ; k++ ){
                sum += A[i * size + k] * B[k * size + j];
            }
            C[i * size + j] = sum;
        }
    }
}

// Function to initialize matrices with random values
void initializeMatrix( double* mat, int size ){
    for( int i = 0 ; i < size ; i++ ){
        mat[i] = static_cast<double>( rand() % 10 );
    }
}

// Function to print matrices
void printMatrix( double* mat, int size ){
    for( int i = 0 ; i < size ; i++ ){
        for( int j = 0 ; j < size ; j++ ){
            std::cout << mat[i * size + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(){
    int size = N * N;
    
    // Allocate memory for matrices
    double* A = new double[ size ];
    double* B = new double[ size ];
    double* C = new double[ size ];

    // Initialize matrices
    initializeMatrix( A, size );
    initializeMatrix( B, size );

    // Print input matrices
    std::cout << "Matrix A:" << std::endl;
    printMatrix( A, N );
    
    std::cout << "Matrix B:" << std::endl;
    printMatrix( B, N );

    // Perform matrix multiplication with OpenMP
    double start_time = omp_get_wtime();
    matrixMultiplicationOMP( A, B, C, N );
    double end_time = omp_get_wtime();

    // Print result
    std::cout << "Matrix C (A x B):" << std::endl;
    printMatrix( C, N );

    std::cout << "Execution Time: " << (end_time - start_time) << " seconds" << std::endl;

    // Free memory
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
