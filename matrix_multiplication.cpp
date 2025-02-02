#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const int MAX_RAND = 1000000;
#ifdef DEBUG
const int MULTIPLICATION_COUNT = 1; // number of multiplications
#else
const int MULTIPLICATION_COUNT = 100; // number of multiplications
#endif

void Parse_arguments( int argc, char* argv[], int* thread_count, int* m, int* n, int* k, int* method );
void Display_usage( char* prog_name );
void Initialize_matrix( double mat[], int m, int n );
void Transpose( double mat[], double mat_t[], int m, int n, int thread_count );
void Display_matrix( double mat[], int m, int n, char* title );

void Matrix_multiply_standard( double A[], double B[], double C[], int m, int n, int k, int thread_count );
void Matrix_multiply_optimized( double A[], double B[], double C[], double BT[], int m, int n, int k, int thread_count );

int main( int argc, char* argv[] ){
    int thread_count, m, n, k, method;
    Parse_arguments( argc, argv, &thread_count, &m, &n, &k, &method );

    double *A, *B, *C, *BT;
    A = ( double* )malloc( m * n * sizeof( double ) );
    B = ( double* )malloc( n * k * sizeof( double ) );
    C = ( double* )malloc( m * k * sizeof( double ) );
    BT = ( double* )malloc( k * n * sizeof( double ) );

    Initialize_matrix( A, m, n );
    Initialize_matrix( B, n, k );
#ifdef DEBUG
    Display_matrix( A, m, n, "Matrix A" );
    Display_matrix( B, n, k, "Matrix B" );
#endif

    switch ( method ){
    case 1:
        Matrix_multiply_standard( A, B, C, m, n, k, thread_count );
        break;
    case 2:
        Matrix_multiply_optimized( A, B, C, BT, m, n, k, thread_count );
        break;
    }

#ifdef DEBUG
    Display_matrix( C, m, k, "Resulting Matrix" );
#endif

    free( A );
    free( B );
    free( C );
    free( BT );

    return 0;
}

void Parse_arguments( int argc, char* argv[], int* thread_count, int* m, int* n, int* k, int* method ){
    if( argc != 6 )
        Display_usage( argv[ 0 ] );
    
    *thread_count = strtol( argv[ 1 ], NULL, 10 );
    *m = strtol( argv[ 2 ], NULL, 10 );
    *n = strtol( argv[ 3 ], NULL, 10 );
    *k = strtol( argv[ 4 ], NULL, 10 );
    *method = strtol( argv[ 5 ], NULL, 10 );
    if( *thread_count <= 0 || *m <= 0 || *n <= 0 || *k <= 0 || ( *method != 1 && *method != 2 ) )
        Display_usage( argv[ 0 ] );
}

void Display_usage( char* prog_name ){
    fprintf( stderr, "Usage: %s <thread_count> <m> <n> <k> <method>\n", prog_name );
    exit( 0 );
}

void Initialize_matrix( double mat[], int m, int n ){
    for( int i = 0 ; i < m ; i++ )
        for( int j = 0 ; j < n ; j++ )
            mat[ i*n + j ] = ( rand() % MAX_RAND )/( MAX_RAND / 10.0 );
}

void Transpose( double mat[], double mat_t[], int m, int n, int thread_count ){
#pragma omp parallel for num_threads( thread_count ) \
    default( none )shared( mat, mat_t, m, n )
    for( int i = 0 ; i < m ; i++ ){
        for( int j = 0 ; j < n ; j++ ){
            mat_t[ j*m + i ] = mat[ i*n + j ];
        }
    }
}

void Display_matrix( double mat[], int m, int n, char* title ){
    printf( "%s\n", title );
    for( int i = 0 ; i < m ; i++ ){
        for( int j = 0 ; j < n ; j++ )
            printf( "%f ", mat[ i*n + j ] );
        printf( "\n" );
    }
}

void Matrix_multiply_standard( double A[], double B[], double C[], int m, int n, int k, int thread_count ){
    double start, finish, temp, avg_elapsed = 0.0;
    for( int count = 0; count < MULTIPLICATION_COUNT; count++ ){
        start = omp_get_wtime();
#pragma omp parallel for num_threads( thread_count ) \
    default( none ) private( temp ) shared( A, B, C, m, n, k )
        for( int i = 0 ; i < m ; i++ ){
            for( int j = 0 ; j < k ; j++ ){
                temp = 0.0;
                for( int l = 0 ; l < n ; l++ ){
                    temp += A[ i*n + l ]*B[ l*k + j ];
                }
                C[ i*k + j ] = temp;
            }
        }
        finish = omp_get_wtime();
        printf( "[%3d] Elapsed time = %f seconds\n", count+1, finish - start );
        avg_elapsed += ( finish-start ) / MULTIPLICATION_COUNT;
    }

    printf( "Average elapsed time: %.6f seconds\n", avg_elapsed );
}

void Matrix_multiply_optimized( double A[], double B[], double C[], double BT[], int m, int n, int k, int thread_count ){
    double start, finish, temp, avg_elapsed = 0.0;
    for( int count = 0 ; count < MULTIPLICATION_COUNT ; count++ )
        start = omp_get_wtime();
        Transpose( B, BT, n, k, thread_count );
#pragma omp parallel for num_threads( thread_count ) \
    default( none ) private( temp ) shared( A, BT, C, m, n, k )
        for( int i = 0 ; i < m ; i++ ){
            for( int j = 0 ; j < k ; j++ ){
                temp = 0.0;
                for( int l = 0 ; l < n ; l++ ){
                    temp += A[ i*n + l ] * BT[ j*n + l ];
                }
                C[ i*k + j ] = temp;
            }
        }
        finish = omp_get_wtime();
        printf( "[%3d] Elapsed time = %f seconds\n", count+1, finish - start );
        avg_elapsed += ( finish-start ) / MULTIPLICATION_COUNT;
    }

    printf( "Average elapsed time: %.6f seconds\n", avg_elapsed );
}
