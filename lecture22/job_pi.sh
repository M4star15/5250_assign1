#!/bin/bash

# Copy/paste this job script into a text file and submit with the command:
#    sbatch thefilename

#SBATCH --partition=instruction
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --time=0-0:5:0
#SBATCH --mem=4G
#SBATCH --account=s2026.coms.5250.01
#SBATCH --qos=instruction
#SBATCH --job-name="lab22"
#SBATCH --mail-user=mhong07@iastate.edu
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --output="pi_compare_%j.out"
#SBATCH --error="pi_compare_%j.err"

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