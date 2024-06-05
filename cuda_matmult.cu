#include<iostream>
#define tile_width 16

using namespace std;

__global__ void Mat_Mul_Shared(float *d_A, float *d_B, float *d_C, int width) {

	 int i = blockIdx.y * blockDim.y + threadIdx.y;
	 int j = blockIdx.x * blockDim.x + threadIdx.x;

	 __shared__ float M[tile_width][tile_width];
	 __shared__ float N[tile_width][tile_width];

	 int n_tile = width / tile_width;  //number of tiles
	 float value = 0;

	 for(int m=0; m < n_tile; m++ ){  // m loop of tiles
			 // copy from globalb memory to shared memory       
			 M[threadIdx.y][threadIdx.x] = d_A[i*width + m*tile_width + threadIdx.x];
			 N[threadIdx.y][threadIdx.x] = d_B[(m*tile_width + threadIdx.y)*width + j];
			 __syncthreads();  // wait all threads finish copying

			for(int k=0; k < tile_width; k++){  // k loop within a tile, in the m loop of tiles: together loop over width to perfomr the dot product of a row of d_A  and a coloumn of d_B
					value += M[threadIdx.y][k] * N[k][threadIdx.x];  // scalar value is in the register of a thread
			}
			__syncthreads();  // wait ll threads finish partial dot product
	 }

	 d_C[i*width + j] = value;  // assign value in register of every thread to corresponding elements of d_C in global memory

}


int main(){

	int n;
	// cin >> n;
	n = 10000;
	int size = n*n*sizeof(float);

	//float *A, *B, *C;
	float *A = (float *) malloc( size );  // allocate CPU memory
	float *B = (float *) malloc( size );
	float *C = (float *) malloc( size );

	for(int i=0; i<n; i++)
		 for(int j=0; j<n; j++){
				 int idx = i*n + j;
				 A[idx] = rand()%10;
				 B[idx] = rand()%10;
				 C[idx] = 0;
	}


	float *d_A, *d_B, *d_C;

	// CUDA events for timing
    cudaEvent_t start, stop, start_load, stop_load, start_unload, stop_unload;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
	cudaEventCreate(&start_load);
    cudaEventCreate(&stop_load);
	cudaEventCreate(&start_unload);
    cudaEventCreate(&stop_unload);

	// Load

	cudaEventRecord(start_load);  // Record start load time

	cudaMalloc(&d_A, size);  // allocate GPU memory
	cudaMalloc(&d_B, size);
	cudaMalloc(&d_C, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

	int block_width = tile_width;
	int grid_width = ceil(n/block_width);
	// dim3 is a CUDA built-in struct
	dim3 dim_grid(grid_width, grid_width);  // num of blocks = (int(n/16) + 1)^2, efficient if > 6.
	dim3 dim_block(block_width, block_width);  // number of theads per block = 16*16 =256

	cudaEventRecord(stop_load);  // Record stop load time
    cudaEventSynchronize(stop_load);  // Wait for the stop load event to complete
    float milliseconds_load = 0;
    cudaEventElapsedTime(&milliseconds_load, start_load, stop_load);  // Calculate elapsed time for loading
	cout << milliseconds_load << endl;

	// Execution

	cudaEventRecord(start);  // Record start execution time

	Mat_Mul_Shared<<<dim_grid, dim_block>>> (d_A, d_B, d_C, n);  // lauch kenel

	cudaEventRecord(stop);  // Record stop execution time
    cudaEventSynchronize(stop);  // Wait for the stop execution event to complete
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);  // Calculate elapsed time for execution
	cout << milliseconds << endl;

	// Unload

	cudaEventRecord(start_unload);  // Record start execution time

	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

	cudaEventRecord(stop_unload);  // Record stop time
    cudaEventSynchronize(stop_unload);  // Wait for the stop event to complete
    float milliseconds_unload = 0;
    cudaEventElapsedTime(&milliseconds_unload, start_unload, stop_unload);  // Calculate elapsed time
	cout << milliseconds_unload << endl;

//   for(int i=0; i<n; i++)
//      for(int j=0; j<n; j++){
//      cout<<i<<"  "<<j<<"  "<<C[i*n+j]<<endl;
//   } 

	cout << "Time taken for matrix multiplication on GPU: " << endl;
	cout << "Load from CPU to GPU: " << milliseconds_load << " ms" << endl;
	cout << "Execution Time : " << milliseconds << " ms" << endl;
	cout << "Unload to CPU from GPU: " << milliseconds_unload << " ms" << endl;
	cout << "Total Time: " << milliseconds + milliseconds_load + milliseconds_unload << " ms" << endl;

	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	free(A);
	free(B);
	free(C);



}
