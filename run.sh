#!/bin/bash -l
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -o std.out
#SBATCH -J GAn-28
#SBATCH -p cooper -n 1 
#SBATCH -N 1  
#SBATCH --mem-per-cpu=8000M
#SBATCH -t 3-00:00:00

./a.out > log.out
