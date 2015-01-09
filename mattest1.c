#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define m 6
#define n 2
#define rep 100
//calculate A[m][n]*V[n][1]
int main(int argc, char* argv[] )
{
    int i,j,V[n];
    double starttime, endtime, divid_time;
    MPI_Init(&argc, &argv);

    // Find out rank, size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Print off a hello world message
    printf("Hello world from rank %d"
         " out of %d processors\n", world_rank, world_size);
    starttime = MPI_Wtime();
//repeat the calculation for rep times
  for (int i = 0; i < rep; i++)
    {
        /* code */
      
    for(j = 0, V[j]=0; j < n; j++)
    { 
        V[j]=1;
        printf("V[%d]:%d \n",j,V[j]);
      //V[j] = rand() % 10;  
      //printf("V[%d]:%d",j,V[j]);
    }

    if (world_rank == 0) 
    {     //Generate Random matrix A on proc 0
        int A[m][n], C[m];
        for(i = 0; i < m; i++)
           for(j = 0; j < n; j++)
               {
               A[i][j] = i * n + j; //fix the matrix
               //A[i][j] = rand() % 100;  
               printf("A[%d][%d]:%d \n",i,j,A[i][j]); 
               }
        int my_row_size;
        for (i=1; i < world_size; i++)
        {   //distribute the A[m][n] into A[my_row_size][n] according to the proc number
            if (i != world_size - 1)  
                my_row_size = m/world_size;
            if (i == world_size - 1) 
                my_row_size = m - (world_size - 1) * (m/world_size);
            printf("Distribute the matrix to : %d \n", i);
            MPI_Send(&A[(m/world_size)*i][0], my_row_size*n, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Distribute the matrix to : %d End End End End \n", i);
        }
        my_row_size = m/world_size;
        for(i = 0; i < my_row_size; i++)
        {
            C[i]=0;
            for(j = 0 ; j < n; j++) 
            {
               printf("A[%d][%d]:%d \n", i, j, A[i][j]);//ensure the sub matrix AA[][] received correctly
               C[i]+=A[i][j]*V[j];
            } 
            printf("C[%d]:%d \n",i,C[i]);
        } 
        for (i=1; i < world_size; i++)
        {
            if (i != world_size - 1)  
                my_row_size =int(m/world_size) ;
            if (i == world_size - 1) 
                my_row_size = m - (world_size - 1) * int(m/world_size);
            MPI_Recv(&C[(m/world_size)*i], my_row_size, MPI_INT, i,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (i=0; i < m ; i++)
            printf("Final : C[%d]:%d \n",i,C[i]);

    }
    //divid_time = MPI_Wtime();//distribute and initiate time in the loop
    if (world_rank != 0) 
    {
        int my_row_size;
        if (i != world_size - 1) { 
            my_row_size =int(m/world_size);
        }        
        if (i == world_size - 1) { 
            my_row_size = m - (world_size -1)*(int(m/world_size));
        }
        int AA[my_row_size][n];
        int C[my_row_size];   
        //initiate the sub matrix AA[my_row_size][n]= 0 and vector V[n]=(1:n)
        for(i = 0; i < my_row_size; i++)
            for(j = 0; j < n; j++) 
                AA[i][j] = 0; 
        printf("%d: Recieving the matrix!!! \n", world_rank);
        MPI_Recv(&AA[0][0], my_row_size*n, MPI_INT, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);      
        printf("%d: Recieving the matrix!!! End End End End \n", world_rank);     
        //compute the multiplication in each process
        for(i = 0; i < my_row_size; i++)
        {
            C[i]=0;
            for(j = 0 ; j < n; j++) 
            {
               printf("AA[%d][%d]:%d \n", i, j, AA[i][j]);//ensure the sub matrix AA[][] received correctly
               C[i]+=AA[i][j]*V[j];
            } 
            printf("C[%d]:%d \n",i,C[i]);
        }//Send the computed matrix to proc 0

        MPI_Send(&C[0], my_row_size, MPI_INT, 0,0, MPI_COMM_WORLD);

    }
    MPI_Bcast(&C[0],n,MPI_INT,0,MPI_COMM_WORLD)
    //check whether the C[n] is calculated
    for(i=0,i<n,i++) printf("C[%d] %d \t", i , C[i] );
}//end of repeat
    
    endtime = MPI_Wtime();
    printf("Total TIME: %f seconds \n ", endtime-starttime);
   // printf("Calculation TIME: %f seconds \n ", endtime-divid_time);

    MPI_Finalize();
}
