#!/bin/bash
#SBATCH --job-name=pi_compare
#SBATCH --output=pi_compare_%j.out
#SBATCH --error=pi_compare_%j.err
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1

module purge
module load openmpi/5.0.8-mybw4ys

echo "Compiling codes..."
mpicc mpi_pi.c -o mpi_pi
gcc -fopenmp omp_pi.c -o omp_pi

echo "===== OpenMP ====="
export OMP_NUM_THREADS=2
./omp_pi 10000000

echo "===== MPI ====="
export HOME=/home/mhong07
mpirun --oversubscribe --bind-to none -np 2 ./mpi_pi 10000000
