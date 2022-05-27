#!/bin/bash
#SBATCH --partition=hef
#SBATCH --output=std_%A_%a.txt
#SBATCH --mem=2000M
#SBATCH --time=5-0:00:00
cd ~/PhD-Quantum-Gravity-A-Silva/example/cdt