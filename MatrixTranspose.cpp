void MatrixTranpose( int w, int h, double* i_matrix, double* o_matrix ){
#pragma omp parallel

#pragma omp for collapse(2)
        for( int i = 0 ; i < h ; i++ ){
		    for( int j = 0 ; j < w ; j++ ){
			    o_matrix[ ( j * h ) + i ] = i_matrix[ ( i * w ) + j ];
		    }
	    }

}   
