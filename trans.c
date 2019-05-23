/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int i,j,x,y,a1,b1,c1,d1,a,b,c,d;
	if(N==32)
		for(x=0;x<N;x+=8)
			for(y=0;y<M;y+=8){
				if(x==y){
					for(i=x;i<x+8;i++){
						a=A[i][i];
						for(j=y;j<y+8;j++){
							if(i!=j)
								B[j][i] = A[i][j];
						}
						B[i][i] = a;
					}
				}
				else{
					for(i=x;i<x+8;i++){
						for(j=y;j<y+8;j++){
							B[j][i] = A[i][j];
						}
					}

				}
			}
	else if(N==64){

		for(x=0;x<N;x+=8)
			for(y=0;y<M;y+=8){
					a1 = A[x][y+4];
					b1 = A[x][y+5];
					c1 = A[x][y+6];
					d1 = A[x][y+7];
				for(i=x;i<x+8;i++){
					a = A[i][y];
					b = A[i][y+1];
					c = A[i][y+2];
					d = A[i][y+3];

					B[y][i] = a;
					B[y+1][i] = b;
					B[y+2][i] = c;
					B[y+3][i] = d;


				}
				for(i=x+7;i>x;i--){
					a = A[i][y+4];
					b = A[i][y+5];
					c = A[i][y+6];
					d = A[i][y+7];

					B[y+4][i] = a;
					B[y+5][i] = b;
					B[y+6][i] = c;
					B[y+7][i] = d;
				}
				B[y+4][x]=a1;
				B[y+5][x]=b1;
				B[y+6][x]=c1;
				B[y+7][x]=d1;
			}
	}
	else{
		for(x=0;x<N;x+=12)
			for(y=0;y<M;y+=4){
				for(i=x;i<x+12 && i<N;i++){
					for(j=y;j<y+4&&j<M;j++){
						B[j][i] = A[i][j];
					}
				}
			}

	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, tmp;

	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			tmp = A[i][j];
			B[j][i] = tmp;
		}
	}    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
	/* Register your solution function */
	registerTransFunction(transpose_submit, transpose_submit_desc); 

	/* Register any additional transpose functions */
	registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < M; ++j) {
			if (A[i][j] != B[j][i]) {
				return 0;
			}
		}
	}
	return 1;
}

