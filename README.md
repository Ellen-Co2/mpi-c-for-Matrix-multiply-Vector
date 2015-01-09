## MPI repeat calculate matrix multiplication
1. Create the matrix A[M]*[N] in node0
2. Create global vector V[N][1]=1
3. Distribute the matrix according to the number of process
4. Calculate A[my_row_size][N]* V[N][1]= C[my_row_size] in every process
5. Send the C[my_row_size] to node0 then broad cast to other process
6. print the time costed by the step 2-5
 *** to change the repeat times modify the rep
