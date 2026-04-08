#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(void)
{
    int comm_sz, my_rank;
    int token;
    int source, dest;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Need at least 2 processes for a meaningful baton pass
    if (comm_sz < 2)
    {
        if (my_rank == 0)
        {
            fprintf(stderr, "Please run with at least 2 MPI processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (my_rank == 0)
    {
        // Start
        token = 1000;
        dest = 1;

        MPI_Send(&token, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        printf("Rank %d sent token %d to Rank %d\n", my_rank, token, dest);

        // Receive final token back from last rank 
        source = comm_sz - 1;
        MPI_Recv(&token, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received token %d from Rank %d\n", my_rank, token, source);

        printf("Final token at Rank 0 = %d\n", token);
    }
    else
    {
        // Everyone else receives from previous rank
        source = my_rank - 1;
        MPI_Recv(&token, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received token %d from Rank %d\n", my_rank, token, source);

        // Increment
        token += 1;

        // Send to next rank, or back to rank 0 if last rank
        if (my_rank == comm_sz - 1)
        {
            dest = 0;
        }
        else
        {
            dest = my_rank + 1;
        }

        MPI_Send(&token, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        printf("Rank %d sent token %d to Rank %d\n", my_rank, token, dest);
    }

    MPI_Finalize();
    return 0;
}