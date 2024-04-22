#!/bin/bash -l
#SBATCH -D ./
#SBATCH --export=ALL
#SBATCH -o std.out
#SBATCH -J docking 
#SBATCH -p cooper  -n 1
#SBATCH -N 1   
#SBATCH -t 1-00:00:00
hostname
   ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_receptor4.py -r protein.pdb 
 ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_ligand4.py -l ligand.pdb 
 ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_gpf4.py -l ligand.pdbqt -r protein.pdbqt  -y 
 ~/AutoDock/autogrid4 -p protein.gpf 
 ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_dpf4.py -l ligand.pdbqt  -r protein.pdbqt 
 sed -i 's/2500000/2500000/g' ligand_protein.dpf  
 sed -i 's/ga_run 10/ga_run 7/g' ligand_protein.dpf 
  ~/AutoDock/autodock4  -p ligand_protein.dpf -l scoring_result.log 
   