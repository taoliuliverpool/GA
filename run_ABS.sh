#!/bin/bash -l
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -o std.out
#SBATCH -J ABS 
#SBATCH -p cooper  -n 1
#SBATCH -N 1   
#SBATCH -t 1-00:00:00
hostname
 export XTBHOME="~/Xtb5/xtb_exe/ " 
 export MKL_NUM_THREADS=1 
 export OMP_THREAD_LIMIT=1 
 export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 
 export OMP_STACKSIZE=500m 
  ulimit -s unlimited 
 ~/Xtb5/xtb_exe/stda_1.6 -xtb -e 7   > 1.outABS 
