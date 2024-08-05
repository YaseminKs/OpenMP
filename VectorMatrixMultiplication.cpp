void VectorMatrixMultiplication( int w, int h, double *i_matrix, double *i_vector, double *o_vector, double bias ){
#pragma omp parallel 

#pragma omp for collapse(1)
        for( int i = 0 ; i < h ; i++ ){
		    o_vector[ i ] = 0.0;

		    for( int j = 0 ; j < w ; j++ ){
			    o_vector[ i ] += i_vector[ j ] * i_matrix[( i * w ) + j ] + bias;
		    }
	    }
    }
